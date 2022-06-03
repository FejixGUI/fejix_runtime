#ifndef _FEJIX_EVENTS_H_
#define _FEJIX_EVENTS_H_

#include <stdint.h>

// Events
#define FJ_EVENT_CLOSE          1
#define FJ_EVENT_RESIZE         2

// Responses of event handlers
#define FJ_EXIT                 1


struct FjResizeEvent {
    uint32_t width;
    uint32_t height;
};


/**
 * @brief Global Fejix event
 * 
 * @details This structure is passed to FjEventHandler() and holds information
 * about a global GUI event generated by Fejix.
 */
struct FjEvent {
    uint32_t eventType;
    
    union {
        struct FjResizeEvent resizeEvent;
    };
};


#endif // _FEJIX_EVENTS_H_