/*
 * AppConfig.h
 * Ramesh Marikhu < marikhu@gmail.com >
 */

#ifndef CLASSCONFIG_H_
#define CLASSCONFIG_H_

#define APP_DEBUG_MODE                           false

// NOTE: Height is fixed but width will vary as per aspect ratio
// If the size of the image is less than resized value, then the actual image
// will be used.
#define APP_FRAMERATE_IN_VIDEO_CLIP              30

// Increasing this value could just miss the subsequent violations
// Also, it is essential that it is 1, so that one of the frames will have good
// snapshot of the license plate
#define APP_NUM_FRAMES_SKIP_INTERVAL             0       //1

// Write timestamp on image // Accurate to 1 sec
#define APP_WRITE_TIMESTAMP_ON_HD_IMAGE          1

// Detection and Processing areas (Set to false by default - Restricts to 1 violation area and 1 processing area)
#define APP_ENABLE_MULTIPLE_PROCESSING_AREAS     false

// Blob Filtering
#define APP_MIN_FILTER_PERIMETER                 2        // 320 x 240
#define APP_MAX_FILTER_PERIMETER                 20
#define APP_MIN_FILTER_AREA                      5
#define APP_MAX_FILTER_AREA                      60

// Enable selecting first frame
#define APP_ENABLE_SELECTING_FIRST_FRAME         false   // SET FALSE
#define APP_ENABLE_SKIPPING_FRAMES_MANUALLY      false
#define APP_ENABLE_PAUSE                         true
#define APP_ENABLE_SLOWDOWN                      true

#define APP_TIME_DIFF_BETW_FRAMES                33.0f     // 33 ms per frame

#define APP_DEPLOY                               false    //true //
#if APP_DEPLOY
#   ifdef APP_DISPLAY_IMAGES
#       undef APP_DISPLAY_IMAGES
#       define APP_DISPLAY_IMAGES                false
#   endif
#   ifdef APP_ENABLE_PAUSE
#       undef APP_ENABLE_PAUSE
#       define APP_ENABLE_PAUSE                  false
#   endif
#   ifdef APP_ENABLE_SKIPPING_FRAMES_MANUALLY
#       undef APP_ENABLE_SKIPPING_FRAMES_MANUALLY
#       define APP_ENABLE_SKIPPING_FRAMES_MANUALLY   false
#   endif
#endif

#endif // #ifndef CLASSCONFIG_H_
