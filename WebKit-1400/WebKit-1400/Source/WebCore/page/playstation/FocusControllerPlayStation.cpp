/*
 * Copyright (C) 2018 Sony Interactive Entertainment Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "FocusController.h"

#include "Element.h"
#include "HTMLElement.h"
#include "HTMLInputElement.h"
#include "HTMLTextAreaElement.h"
#include "Page.h"

namespace WebCore {

bool FocusController::hasNextEditableElement(Element* element)
{
    Element* e = nextEditableElement(element);
    return (e && (e != element));
}

bool FocusController::hasPreviousEditableElement(Element* element)
{
    Element* e = previousEditableElement(element);
    return (e && (e != element));
}

static bool isEditableElement(Element* element)
{
    if (!is<HTMLElement>(element))
        return false;

    // Target tag: input.
    if (element->hasTagName(HTMLNames::inputTag)) {
        HTMLInputElement* input = downcast<HTMLInputElement>(element);
        return (input->isTextField() && !input->isReadOnly());
    }

    // Target tag: textarea.
    if (element->hasTagName(HTMLNames::textareaTag)) {
        HTMLTextAreaElement* textarea = downcast<HTMLTextAreaElement>(element);
        return !textarea->isReadOnly();
    }

    // Target tag: Other than input and textarea (div, p, etc).
    // isContentEditable:
    //   Returns whether the element is editable by the contenteditable attribute or -webkit-user-modify.
    return downcast<HTMLElement>(element)->isContentEditable();
}

Element* FocusController::nextEditableElement(Element* element)
{
    if (!element)
        return nullptr;

    Element* nextElement = element;
    FocusController& focusController = { element->document().page()->focusController() };

    do {
        nextElement = focusController.nextFocusableElement(*nextElement);
    } while (nextElement && !(nextElement->isHTMLElement() && !nextElement->isFrameOwnerElement() && isEditableElement(nextElement)));

    if (!nextElement)
        return nullptr;

    return nextElement;
}

Element* FocusController::previousEditableElement(Element* element)
{
    if (!element)
        return nullptr;

    Element* previousElement = element;
    FocusController& focusController = { element->document().page()->focusController() };

    do {
        previousElement = focusController.previousFocusableElement(*previousElement);
    } while (previousElement && !(previousElement->isHTMLElement() && !previousElement->isFrameOwnerElement() && isEditableElement(previousElement)));

    if (!previousElement)
        return nullptr;

    return previousElement;
}

}
