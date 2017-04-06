#include "OSXHelper.h"
#include <CoreFoundation/CoreFoundation.h>

OSXHelper::OSXHelper()
{
}

QString OSXHelper::getMacPath(int toggle)
{
        char path[1024];
        CFBundleRef mainBundle = CFBundleGetMainBundle();


        CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);


        CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, kCFURLPOSIXPathStyle);

        CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

        CFRelease(mainBundleURL);
        CFRelease(cfStringRef);

    if(toggle==0)
        return QString(path)+"/Contents/Resources";
    else if(toggle==1)
        return QString(path)+"/Contents/MacOS/";
}
