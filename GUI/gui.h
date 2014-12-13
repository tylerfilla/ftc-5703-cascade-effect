
/**
 * 
 * 
 * The Doppler Effect
 * FTC #5703
 * 
 * gui.h
 * 
 * Provides a GUI engine for the NXT with several basic widgets and a simple API
 * 
 * 
 */

#ifndef __GUI__
#define __GUI__

/* Serviceable definitions */

// Table sizes
#define GUI_NUM_ELEMENTS               16
#define GUI_NUM_EVENTS_PER_ELEMENT     8
#define GUI_NUM_BYTES_PER_EVENT        16
#define GUI_NUM_PROPERTIES_PER_ELEMENT 10
#define GUI_NUM_BYTES_PER_PROPERTY     16

// Event queue size
#define GUI_EVENT_QUEUE_SIZE 16

// NXT buttons
#define GUI_NXT_BUTTON_COOLDOWN_TIME 400
#define GUI_NXT_NUM_EXIT_CLICKS      3

/* Internal definitions */

// States
#define GUI_STATE_STOPPED 0
#define GUI_STATE_RUNNING 1

// NXT buttons
#define GUI_NXT_BUTTON_ENTER (TNxtButtons) 3
#define GUI_NXT_BUTTON_EXIT  (TNxtButtons) 0
#define GUI_NXT_BUTTON_LEFT  (TNxtButtons) 2
#define GUI_NXT_BUTTON_RIGHT (TNxtButtons) 1

// Element types
#define GUI_ELEMENT_BUTTON       0
#define GUI_ELEMENT_BUTTON_RADIO 1
#define GUI_ELEMENT_CHECKBOX     2
#define GUI_ELEMENT_SPINNER      3
#define GUI_ELEMENT_STRING       4
#define GUI_ELEMENT_TEXT         5

// Properties for button element
#define GUI_PROPERTY_BUTTON_HEIGHT 0
#define GUI_PROPERTY_BUTTON_POS_X  1
#define GUI_PROPERTY_BUTTON_POS_Y  2
#define GUI_PROPERTY_BUTTON_SHAPE  3
#define GUI_PROPERTY_BUTTON_TEXT   4
#define GUI_PROPERTY_BUTTON_WIDTH  5

// Shape property enums for button element
#define GUI_PROPERTY_BUTTON_SHAPE_ELLIPSE   0
#define GUI_PROPERTY_BUTTON_SHAPE_RECTANGLE 1

// Events for button element
#define GUI_EVENT_BUTTON_BLUR  0
#define GUI_EVENT_BUTTON_FOCUS 1
#define GUI_EVENT_BUTTON_PRESS 2

// Properties for checkbox element
#define GUI_PROPERTY_CHECKBOX_CHECKED 0
#define GUI_PROPERTY_CHECKBOX_POS_X   1
#define GUI_PROPERTY_CHECKBOX_POS_Y   2

// Events for checkbox element
#define GUI_EVENT_CHECKBOX_BLUR    0
#define GUI_EVENT_CHECKBOX_CHECK   1
#define GUI_EVENT_CHECKBOX_FOCUS   2
#define GUI_EVENT_CHECKBOX_TOGGLE  3
#define GUI_EVENT_CHECKBOX_UNCHECK 4

// Properties for radio button element
#define GUI_PROPERTY_BUTTON_RADIO_CHANNEL 0
#define GUI_PROPERTY_BUTTON_RADIO_CHECKED 1
#define GUI_PROPERTY_BUTTON_RADIO_POS_X   2
#define GUI_PROPERTY_BUTTON_RADIO_POS_Y   3

// Events for radio button element
#define GUI_EVENT_BUTTON_RADIO_BLUR    0
#define GUI_EVENT_BUTTON_RADIO_CHECK   1
#define GUI_EVENT_BUTTON_RADIO_FOCUS   2
#define GUI_EVENT_BUTTON_RADIO_UNCHECK 3

// Properties for spinner element
#define GUI_PROPERTY_SPINNER_HEIGHT  0
#define GUI_PROPERTY_SPINNER_MAXIMUM 1
#define GUI_PROPERTY_SPINNER_MINIMUM 2
#define GUI_PROPERTY_SPINNER_POS_X   3
#define GUI_PROPERTY_SPINNER_POS_Y   4
#define GUI_PROPERTY_SPINNER_STEP    5
#define GUI_PROPERTY_SPINNER_SUFFIX  6
#define GUI_PROPERTY_SPINNER_VALUE   7
#define GUI_PROPERTY_SPINNER_WIDTH   8

// Events for spinner element
#define GUI_EVENT_SPINNER_BLUR       0
#define GUI_EVENT_SPINNER_CHANGE     1
#define GUI_EVENT_SPINNER_DOWN       2
#define GUI_EVENT_SPINNER_FOCUS      3
#define GUI_EVENT_SPINNER_UP         4

// Properties for string element
#define GUI_PROPERTY_STRING_POS_X 0
#define GUI_PROPERTY_STRING_POS_Y 1
#define GUI_PROPERTY_STRING_SIZE  2
#define GUI_PROPERTY_STRING_TEXT  3

// Size property enums for string element
#define GUI_PROPERTY_STRING_SIZE_BIG    0
#define GUI_PROPERTY_STRING_SIZE_NORMAL 1

// Properties for text element
#define GUI_PROPERTY_TEXT_ALIGN 0
#define GUI_PROPERTY_TEXT_LINE  1
#define GUI_PROPERTY_TEXT_SIZE  2
#define GUI_PROPERTY_TEXT_TEXT  3

// Align property enums for text element
#define GUI_PROPERTY_TEXT_ALIGN_CENTER 0
#define GUI_PROPERTY_TEXT_ALIGN_LEFT   1

// Size property enums for text element
#define GUI_PROPERTY_TEXT_SIZE_BIG    0
#define GUI_PROPERTY_TEXT_SIZE_NORMAL 1

/* Types */

typedef short TGuiElement;

/* Prototyping */

// GUI control
task        gui_task();
int         gui_get_state();
void        gui_begin();
void        gui_end();
void        gui_disable_auto_render();
void        gui_enable_auto_render();
void        gui_mark_needs_render();
TGuiElement gui_element_create(int type);
void        gui_element_remove(TGuiElement element);
bool        gui_element_check_focusable(TGuiElement element);
void        gui_element_focus(TGuiElement element);
void        gui_element_activate(TGuiElement element);
long        gui_property_get(TGuiElement element, int property);
void        gui_property_get_str(TGuiElement element, int property, char* data);
void        gui_property_set(TGuiElement element, int property, long data);
void        gui_property_set_str(TGuiElement element, int property, char* data);
bool        gui_event_get(char* tag);
void        gui_event_invoke(TGuiElement element, int event);
void        gui_event_register(TGuiElement element, int event, char* tag);
void        gui_event_unregister(TGuiElement element, int event);

/* Current state */

// GUI data tables
signed char guiElementTable[GUI_NUM_ELEMENTS];
signed char guiElementEventTable[GUI_NUM_ELEMENTS*GUI_NUM_EVENTS_PER_ELEMENT*GUI_NUM_BYTES_PER_EVENT];
signed char guiElementPropertyTable[GUI_NUM_ELEMENTS*GUI_NUM_PROPERTIES_PER_ELEMENT*GUI_NUM_BYTES_PER_PROPERTY];

// Event queue
signed char guiElementEventQueue[GUI_EVENT_QUEUE_SIZE*GUI_NUM_BYTES_PER_EVENT];
int         guiElementEventQueueSize = 0;

// Element interactivity
TGuiElement guiElementActive = (TGuiElement) -1;
TGuiElement guiElementFocus  = (TGuiElement) -1;

int guiNxtButtonCooldown = 0;

// State
int guiState = GUI_STATE_STOPPED;

// Flags
bool guiAutoRender  = true;
bool guiNeedsRender = false;

/* GUI task */

/**
 * 
 * Main GUI engine task. This task must not be started directly.
 * 
 */
task gui_task() {
    while (guiState == GUI_STATE_RUNNING) {
        // Rendering
        if (guiNeedsRender) {
            eraseDisplay();
            
            for (TGuiElement element = 0; element < GUI_NUM_ELEMENTS; element++) {
                int elementType = guiElementTable[element];
                
                if (elementType == GUI_ELEMENT_BUTTON) {
                    /* Read properties */
                    
                    char height = gui_property_get(element, GUI_PROPERTY_BUTTON_HEIGHT);
                    char posX   = gui_property_get(element, GUI_PROPERTY_BUTTON_POS_X);
                    char posY   = gui_property_get(element, GUI_PROPERTY_BUTTON_POS_Y);
                    char shape  = gui_property_get(element, GUI_PROPERTY_BUTTON_SHAPE);
                    char width  = gui_property_get(element, GUI_PROPERTY_BUTTON_WIDTH);
                    
                    char text[GUI_NUM_BYTES_PER_PROPERTY];
                    gui_property_get_str(element, GUI_PROPERTY_BUTTON_TEXT, text);
                    
                    /* Render */
                    
                    if (shape == GUI_PROPERTY_BUTTON_SHAPE_RECTANGLE) {
                        displayStringAt(posX + (width - strlen(text)*6)/2, posY + (height + 7)/2, text);
                        drawRect(posX, posY, posX + width, posY + height);
                        
                        if (element == guiElementFocus) {
                            drawRect(posX - 1, posY - 1, posX + width + 1, posY + height + 1);
                        }
                    } else if (shape == GUI_PROPERTY_BUTTON_SHAPE_ELLIPSE) {
                        displayStringAt(posX + (width - strlen(text)*6)/2, posY + (height + 7)/2, text);
                        drawEllipse(posX, posY, posX + width, posY + height);
                        
                        if (element == guiElementFocus) {
                            drawEllipse(posX, posY + 1, posX + width, posY + height - 1);
                            drawEllipse(posX + 1, posY, posX + width - 1, posY + height);
                        }
                    }
                } else if (elementType == GUI_ELEMENT_BUTTON_RADIO) {
                    /* Read properties */
                    
                    bool checked = (bool) gui_property_get(element, GUI_PROPERTY_BUTTON_RADIO_CHECKED);
                    char posX    = gui_property_get(element, GUI_PROPERTY_BUTTON_RADIO_POS_X);
                    char posY    = gui_property_get(element, GUI_PROPERTY_BUTTON_RADIO_POS_Y);
                    
                    /* Render */
                    
                    if (checked) {
                        fillEllipse(posX + 2, posY + 2, posX + 6, posY + 6);
                    }
                    
                    drawEllipse(posX, posY, posX + 8, posY + 8);
                    
                    if (element == guiElementFocus) {
                        drawEllipse(posX - 1, posY, posX + 9, posY + 8);
                        drawEllipse(posX, posY - 1, posX + 8, posY + 9);
                    }
                } else if (elementType == GUI_ELEMENT_CHECKBOX) {
                    /* Read properties */
                    
                    bool checked = (bool) gui_property_get(element, GUI_PROPERTY_CHECKBOX_CHECKED);
                    char posX    = gui_property_get(element, GUI_PROPERTY_CHECKBOX_POS_X);
                    char posY    = gui_property_get(element, GUI_PROPERTY_CHECKBOX_POS_Y);
                    
                    /* Render */
                    
                    if (checked) {
                        displayStringAt(posX + 3, posY + 9, "x");
                    }
                    
                    drawRect(posX, posY, posX + 10, posY + 10);
                    
                    if (element == guiElementFocus) {
                        drawRect(posX - 1, posY - 1, posX + 11, posY + 11);
                    }
                } else if (elementType == GUI_ELEMENT_SPINNER) {
                    /* Read properties */
                    
                    char  height   = gui_property_get(element, GUI_PROPERTY_SPINNER_HEIGHT);
                    char  posX     = gui_property_get(element, GUI_PROPERTY_SPINNER_POS_X);
                    char  posY     = gui_property_get(element, GUI_PROPERTY_SPINNER_POS_Y);
                    long  value    = gui_property_get(element, GUI_PROPERTY_SPINNER_VALUE);
                    char  width    = gui_property_get(element, GUI_PROPERTY_SPINNER_WIDTH);
                    
                    char suffix[GUI_NUM_BYTES_PER_PROPERTY];
                    gui_property_get_str(element, GUI_PROPERTY_SPINNER_SUFFIX, suffix);
                    
                    /* Render */
                    
                    displayStringAt(posX + (width - 4*6)/2, posY + (height + 7)/2, "%d", (int) value);
                    
                    drawRect(posX, posY, posX + width, posY + height);
                    drawRect(posX + width - height/2, posY + height/2, posX + width, posY + height);
                    drawRect(posX + width - height/2, posY, posX + width, posY + height/2);
                    
                    drawLine(posX + width - height/2 + 2, posY + height/2 + 2, posX + width - height/4, posY + height - 2);
                    drawLine(posX + width - 2, posY + height/2 + 2, posX + width - height/4, posY + height - 2);
                    drawLine(posX + width - height/2 + 2, posY + height/2 - 2, posX + width - height/4, posY + 2);
                    drawLine(posX + width - 2, posY + height/2 - 2, posX + width - height/4, posY + 2);
                    
                    if (element == guiElementFocus) {
                        drawRect(posX - 1, posY - 1, posX + width + 1, posY + height + 1);
                    }
                } else if (elementType == GUI_ELEMENT_STRING) {
                    /* Read properties */
                    
                    char posX = gui_property_get(element, GUI_PROPERTY_STRING_POS_X);
                    char posY = gui_property_get(element, GUI_PROPERTY_STRING_POS_Y);
                    char size = gui_property_get(element, GUI_PROPERTY_STRING_SIZE);
                    
                    char text[GUI_NUM_BYTES_PER_PROPERTY];
                    gui_property_get_str(element, GUI_PROPERTY_STRING_TEXT, text);
                    
                    /* Render */
                    
                    if (size == GUI_PROPERTY_STRING_SIZE_BIG) {
                        displayBigStringAt(posX, posY, text);
                    } else if (size == GUI_PROPERTY_STRING_SIZE_NORMAL) {
                        displayStringAt(posX, posY, text);
                    }
                } else if (elementType == GUI_ELEMENT_TEXT) {
                    /* Read properties */
                    
                    char align = gui_property_get(element, GUI_PROPERTY_TEXT_ALIGN);
                    char line  = gui_property_get(element, GUI_PROPERTY_TEXT_LINE);
                    char size  = gui_property_get(element, GUI_PROPERTY_TEXT_SIZE);
                    
                    char text[GUI_NUM_BYTES_PER_PROPERTY];
                    gui_property_get_str(element, GUI_PROPERTY_TEXT_TEXT, text);
                    
                    /* Render */
                    
                    if (align == GUI_PROPERTY_TEXT_ALIGN_CENTER && size == GUI_PROPERTY_TEXT_SIZE_BIG) {
                        displayCenteredBigTextLine(line, text);
                    } else if (align == GUI_PROPERTY_TEXT_ALIGN_CENTER && size == GUI_PROPERTY_TEXT_SIZE_NORMAL) {
                        displayCenteredTextLine(line, text);
                    } else if (align == GUI_PROPERTY_TEXT_ALIGN_LEFT && size == GUI_PROPERTY_TEXT_SIZE_BIG) {
                        displayBigTextLine(line, text);
                    } else if (align == GUI_PROPERTY_TEXT_ALIGN_LEFT && size == GUI_PROPERTY_TEXT_SIZE_NORMAL) {
                        displayTextLine(line, text);
                    }
                }
            }
            
            guiNeedsRender = false;
        }
        
        // User input
        if (nNxtButtonPressed > -1 && guiNxtButtonCooldown == 0) {
            switch (nNxtButtonPressed) {
            case GUI_NXT_BUTTON_ENTER:
                if (guiElementFocus > -1) {
                    // Activate the focused element
                    gui_element_activate(guiElementFocus);
                }
                break;
            case GUI_NXT_BUTTON_EXIT:
                if (guiElementActive > -1) {
                    // Deactivate active element
                    gui_element_activate((TGuiElement) -1);
                }
                break;
            case GUI_NXT_BUTTON_LEFT:
                if (guiElementActive == (TGuiElement) -1) {
                    // No element active, shift focus down
                    for (TGuiElement e = guiElementFocus - 1; e > -1; e--) {
                        if (gui_element_check_focusable(e)) {
                            gui_element_focus(e);
                            break;
                        }
                    }
                } else {
                    switch (guiElementTable[guiElementActive]) {
                    case GUI_ELEMENT_SPINNER:
                        int max   = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_MAXIMUM);
                        int min   = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_MINIMUM);
                        int step  = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_STEP);
                        int value = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_VALUE);
                        
                        value -= step;
                        
                        if (value < min) {
                            value = min;
                        } else if (value > max) {
                            value = max;
                        }
                        
                        gui_property_set(guiElementActive, GUI_PROPERTY_SPINNER_VALUE, value);
                        break;
                    }
                }
                break;
            case GUI_NXT_BUTTON_RIGHT:
                if (guiElementActive == -1) {
                    // No element active, shift focus up
                    for (TGuiElement e = guiElementFocus + 1; e < GUI_NUM_ELEMENTS; e++) {
                        if (gui_element_check_focusable(e)) {
                            gui_element_focus(e);
                            break;
                        }
                    }
                } else {
                    switch (guiElementTable[guiElementActive]) {
                    case GUI_ELEMENT_SPINNER:
                        int max   = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_MAXIMUM);
                        int min   = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_MINIMUM);
                        int step  = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_STEP);
                        int value = gui_property_get(guiElementActive, GUI_PROPERTY_SPINNER_VALUE);
                        
                        value += step;
                        
                        if (value < min) {
                            value = min;
                        } else if (value > max) {
                            value = max;
                        }
                        
                        gui_property_set(guiElementActive, GUI_PROPERTY_SPINNER_VALUE, value);
                        break;
                    }
                }
                break;
            }
            
            guiNeedsRender = true;
            guiNxtButtonCooldown = GUI_NXT_BUTTON_COOLDOWN_TIME;
        }
        
        wait1Msec(10);
        if ((guiNxtButtonCooldown -= 10) < 0) {
            guiNxtButtonCooldown = 0;
        }
    }
}

/* GUI control */

/**
 * 
 * Gets the current state of the GUI engine.
 * 
 * Returns
 *      An integer representing the current GUI engine state
 * 
 */
int gui_get_state() {
    return guiState;
}

/**
 * 
 * Starts the GUI engine and clears any and all pre-existing state.
 * 
 */
void gui_begin() {
    if (guiState == GUI_STATE_STOPPED) {
        // Change state
        guiState = GUI_STATE_RUNNING;
        
        // Reset active/focused elements
        guiElementActive = (TGuiElement) -1;
        guiElementFocus  = (TGuiElement) -1;
        
        // Fill tables and event queue with -1
        memset(guiElementTable,         -1, GUI_NUM_ELEMENTS);
        memset(guiElementEventTable,    -1, GUI_NUM_ELEMENTS*GUI_NUM_EVENTS_PER_ELEMENT*GUI_NUM_BYTES_PER_EVENT);
        memset(guiElementPropertyTable, -1, GUI_NUM_ELEMENTS*GUI_NUM_PROPERTIES_PER_ELEMENT*GUI_NUM_BYTES_PER_PROPERTY);
        memset(guiElementEventQueue,    -1, GUI_EVENT_QUEUE_SIZE*GUI_NUM_BYTES_PER_EVENT);
        
        // Set number of required exit clicks to 3
        nNxtExitClicks = GUI_NXT_NUM_EXIT_CLICKS;
        
        // Start the GUI task
        startTask(gui_task);
    }
}

/**
 * 
 * Stops the GUI engine and clears the screen.
 * 
 */
void gui_end() {
    if (guiState == GUI_STATE_RUNNING) {
        // Change state
        guiState = GUI_STATE_STOPPED;
        
        // Wait for GUI task to exit
        wait1Msec(10);
        
        // Clear the screen
        eraseDisplay();
    }
}

/**
 * 
 * Clears the auto render flag. Once called, the implementation must request
 * render passes using gui_mark_needs_render().
 * 
 */
void gui_disable_auto_render() {
    guiAutoRender = false;
}


/**
 * 
 * Sets the auto render flag (set by default). Once called, render passes will
 * be performed as elements are created, destroyed, and/or modified.
 * 
 */
void gui_enable_auto_render() {
    guiAutoRender = true;
}

/**
 * 
 * Requests that the engine complete a render pass on the next cycle.
 * 
 */
void gui_mark_needs_render() {
    guiNeedsRender = true;
}

/**
 * 
 * Creates a new element of the given type.
 * 
 * Returns
 *      A TGuiElement serving as a handle to the newly created element
 * 
 */
TGuiElement gui_element_create(int type) {
    TGuiElement element = (TGuiElement) -1;
    
    for (TGuiElement e = 0; e < GUI_NUM_ELEMENTS; e++) {
        if (guiElementTable[e] == -1) {
            guiElementTable[e] = type;
            element = e;
            break;
        }
    }
    
    if (guiAutoRender) {
        guiNeedsRender = true;
    }
    
    return element;
}

/**
 * 
 * Removes the element represented by the given handle.
 * 
 */
void gui_element_remove(TGuiElement element) {
    guiElementTable[element] = (TGuiElement) -1;
    
    if (guiAutoRender) {
        guiNeedsRender = true;
    }
}

/**
 * 
 * Checks if the element represented by the given handle can receive user focus.
 * 
 * Returns
 *      A boolean value indicating whether or not the element is focusable
 * 
 */
bool gui_element_check_focusable(TGuiElement element) {
    return guiElementTable[element] == GUI_ELEMENT_BUTTON
        || guiElementTable[element] == GUI_ELEMENT_CHECKBOX
        || guiElementTable[element] == GUI_ELEMENT_BUTTON_RADIO
        || guiElementTable[element] == GUI_ELEMENT_SPINNER;
}

/**
 * 
 * Gives focus to the element represented by the given handle. Passing a value
 * of -1 for the handle removes focus from the currently focused element.
 * 
 */
void gui_element_focus(TGuiElement element) {
    TGuiElement previousFocus = guiElementFocus;
    guiElementFocus = element;
    
    guiNeedsRender = true;
    
    // Invoke blur event on previously focused element
    if (previousFocus > -1) {
        switch (guiElementTable[previousFocus]) {
        case GUI_ELEMENT_BUTTON:
            gui_event_invoke(previousFocus, GUI_EVENT_BUTTON_BLUR);
            break;
        case GUI_ELEMENT_CHECKBOX:
            gui_event_invoke(previousFocus, GUI_EVENT_CHECKBOX_BLUR);
            break;
        case GUI_ELEMENT_BUTTON_RADIO:
            gui_event_invoke(previousFocus, GUI_EVENT_BUTTON_RADIO_BLUR);
            break;
        case GUI_ELEMENT_SPINNER:
            gui_event_invoke(previousFocus, GUI_EVENT_SPINNER_BLUR);
            break;
        }
    }
    
    // Invoke focus event on newly focused element
    if (guiElementFocus > -1) {
        switch (guiElementTable[guiElementFocus]) {
        case GUI_ELEMENT_BUTTON:
            gui_event_invoke(guiElementFocus, GUI_EVENT_BUTTON_FOCUS);
            break;
        case GUI_ELEMENT_CHECKBOX:
            gui_event_invoke(guiElementFocus, GUI_EVENT_CHECKBOX_FOCUS);
            break;
        case GUI_ELEMENT_BUTTON_RADIO:
            gui_event_invoke(guiElementFocus, GUI_EVENT_BUTTON_RADIO_FOCUS);
            break;
        case GUI_ELEMENT_SPINNER:
            gui_event_invoke(guiElementFocus, GUI_EVENT_SPINNER_FOCUS);
            break;
        }
    }
}

/**
 * 
 * Activates the element represented by the given handle. Elements that are
 * activatable perform specific actions (such as button presses). Activated
 * elements capture user interaction and must be deactivated. Certain elements,
 * such as buttons, will immediately deactivate upon activation. Passing a
 * value of -1 for the handle will deactivate the currently activated element.
 * 
 */
void gui_element_activate(TGuiElement element) {
    guiElementActive = element;
    guiNeedsRender = true;
    
    if (element > -1) {
        switch (guiElementTable[element]) {
        case GUI_ELEMENT_BUTTON:
            // Invoke button press event
            gui_event_invoke(element, GUI_EVENT_BUTTON_PRESS);
            
            // Deactivate the active button
            guiElementActive = (TGuiElement) -1;
            break;
        case GUI_ELEMENT_CHECKBOX:
            // Invoke toggle event on the active checkbox
            gui_event_invoke(element, GUI_EVENT_CHECKBOX_TOGGLE);
            
            // Toggle the active checkbox and invoke the respective event
            if ((bool) gui_property_get(element, GUI_PROPERTY_CHECKBOX_CHECKED)) {
                gui_property_set(element, GUI_PROPERTY_CHECKBOX_CHECKED, (char) false);
                gui_event_invoke(element, GUI_EVENT_CHECKBOX_UNCHECK);
            } else {
                gui_property_set(element, GUI_PROPERTY_CHECKBOX_CHECKED, (char) true);
                gui_event_invoke(element, GUI_EVENT_CHECKBOX_CHECK);
            }
            
            // Deactivate the active checkbox
            guiElementActive = (TGuiElement) -1;
            break;
        case GUI_ELEMENT_BUTTON_RADIO:
            // Invoke check event on the active radio button
            gui_property_set(element, GUI_PROPERTY_BUTTON_RADIO_CHECKED, (char) true);
            gui_event_invoke(element, GUI_EVENT_BUTTON_RADIO_CHECK);
            
            // Uncheck all other radio buttons on the same channel and invoke uncheck events
            for (int e = 0; e < GUI_NUM_ELEMENTS; e++) {
                if (guiElementTable[e] == GUI_ELEMENT_BUTTON_RADIO && (TGuiElement) e != element
                    && gui_property_get(element, GUI_PROPERTY_BUTTON_RADIO_CHANNEL) == gui_property_get((TGuiElement) e, GUI_PROPERTY_BUTTON_RADIO_CHANNEL)) {
                    gui_property_set((TGuiElement) e, GUI_PROPERTY_BUTTON_RADIO_CHECKED, (char) false);
                    gui_event_invoke((TGuiElement) e, GUI_EVENT_BUTTON_RADIO_UNCHECK);
                }
            }
            
            // Deactivate the active radio button
            guiElementActive = (TGuiElement) -1;
            break;
        }
    }
}

/**
 * 
 * Gets numerical data associated with the specified property of the element
 * represented by the given handle.
 * 
 * Returns
 *      A long integer containing the property data
 * 
 */
long gui_property_get(TGuiElement element, int property) {
    int base = element*GUI_NUM_PROPERTIES_PER_ELEMENT*GUI_NUM_BYTES_PER_PROPERTY + property*GUI_NUM_BYTES_PER_PROPERTY;
    return (unsigned long) guiElementPropertyTable[base + 0] << 24
         | (unsigned long) guiElementPropertyTable[base + 1] << 16
         | (unsigned long) guiElementPropertyTable[base + 2] << 8
         | (unsigned long) guiElementPropertyTable[base + 3] << 0;
}

/**
 * 
 * Gets string data associated with the specified property of the element
 * represented by the given handle. The property data are written at the given
 * character pointer.
 * 
 */
void gui_property_get_str(TGuiElement element, int property, char* data) {
    memcpy(data, &guiElementPropertyTable[element*GUI_NUM_PROPERTIES_PER_ELEMENT*GUI_NUM_BYTES_PER_PROPERTY + property*GUI_NUM_BYTES_PER_PROPERTY], GUI_NUM_BYTES_PER_PROPERTY);
}

/**
 * 
 * Sets numerical data to the specified property of the element represented by
 * the given handle.
 * 
 */
void gui_property_set(TGuiElement element, int property, long data) {
    int base = element*GUI_NUM_PROPERTIES_PER_ELEMENT*GUI_NUM_BYTES_PER_PROPERTY + property*GUI_NUM_BYTES_PER_PROPERTY;
    guiElementPropertyTable[base + 0] = (unsigned long) (data & (unsigned long) 0xff000000) >> 24;
    guiElementPropertyTable[base + 1] = (unsigned long) (data & (unsigned long) 0x00ff0000) >> 16;
    guiElementPropertyTable[base + 2] = (unsigned long) (data & (unsigned long) 0x0000ff00) >> 8;
    guiElementPropertyTable[base + 3] = (unsigned long) (data & (unsigned long) 0x000000ff) >> 0;
    
    if (guiAutoRender) {
        guiNeedsRender = true;
    }
}

/**
 * 
 * Sets string data to the specified property of the element represented by the
 * given handle. The property data are read from the given character pointer.
 * 
 */
void gui_property_set_str(TGuiElement element, int property, char* data) {
    memcpy(&guiElementPropertyTable[element*GUI_NUM_PROPERTIES_PER_ELEMENT*GUI_NUM_BYTES_PER_PROPERTY + property*GUI_NUM_BYTES_PER_PROPERTY], data, GUI_NUM_BYTES_PER_PROPERTY);
    
    if (guiAutoRender) {
        guiNeedsRender = true;
    }
}

/**
 * 
 * Gets the most recent event tag from the event queue, removing it in the
 * process. The event tag data are written at the given character pointer.
 * 
 * Returns
 *      A boolean value indicating whether or not an event tag was retrieved
 * 
 */
bool gui_event_get(char* tag) {
    if (guiElementEventQueueSize > 0) {
        memcpy(tag, guiElementEventQueue, GUI_NUM_BYTES_PER_EVENT);
        
        for (int i = 0; i < GUI_EVENT_QUEUE_SIZE*GUI_NUM_BYTES_PER_EVENT; i++) {
            if (i > GUI_NUM_BYTES_PER_EVENT - 1) {
                guiElementEventQueue[i - GUI_NUM_BYTES_PER_EVENT] = guiElementEventQueue[i];
            }
        }
        
        guiElementEventQueueSize--;
        
        return true;
    }
    
    return false;
}

/**
 * 
 * Invokes the specified event on the element represented by the given handle.
 * If a tag has been registered to this event using gui_event_register(), it
 * will be appended to the event queue and be made available for retrieval
 * through gui_event_get(). This function is used internally by the GUI engine,
 * but can also be used to simulate user interactions.
 * 
 */
void gui_event_invoke(TGuiElement element, int event) {
    if (guiElementEventQueueSize < GUI_EVENT_QUEUE_SIZE) {
        memcpy(&guiElementEventQueue[(++guiElementEventQueueSize - 1)*GUI_NUM_BYTES_PER_EVENT], &guiElementEventTable[element*GUI_NUM_EVENTS_PER_ELEMENT*GUI_NUM_BYTES_PER_EVENT + event*GUI_NUM_BYTES_PER_EVENT], GUI_NUM_BYTES_PER_EVENT);
    }
}

/**
 * 
 * Registers a tag to the specified event on the element represented by the
 * given handle. This tag will be provided by gui_event_get() when the specified
 * event is invoked.
 * 
 */
void gui_event_register(TGuiElement element, int event, char* tag) {
    memcpy(&guiElementEventTable[element*GUI_NUM_EVENTS_PER_ELEMENT*GUI_NUM_BYTES_PER_EVENT + event*GUI_NUM_BYTES_PER_EVENT], tag, GUI_NUM_BYTES_PER_EVENT);
}

/**
 * 
 * Unregisters the current tag from the specified event on the element
 * represented by the given handle.
 * 
 */
void gui_event_unregister(TGuiElement element, int event) {
    memset(&guiElementEventTable[element*GUI_NUM_EVENTS_PER_ELEMENT*GUI_NUM_BYTES_PER_EVENT + event*GUI_NUM_BYTES_PER_EVENT], 0, GUI_NUM_BYTES_PER_EVENT);
}

#endif // #ifndef __GUI__
