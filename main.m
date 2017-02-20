#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
	UInt32 t = 0xC8F4E849;
	CFAbsoluteTime o;
	if (UCConvertSecondsToCFAbsoluteTime(t, &o) == noErr) {
		// 2082844800
		//o = t - (CFAbsoluteTime)3061152000;
		printf("DIFF: %d\n", (UInt32)o - t);
		CFDateRef d = CFDateCreate(kCFAllocatorDefault, o);
		CFLocaleRef locale = CFLocaleCopyCurrent();
		CFDateFormatterRef fm = CFDateFormatterCreate(kCFAllocatorDefault, locale, kCFDateFormatterFullStyle, kCFDateFormatterMediumStyle);
		
		CFStringRef s = CFDateFormatterCreateStringWithDate(kCFAllocatorDefault, fm, d);
		NSLog(@"s: %@", s);
	}
	
    return NSApplicationMain(argc, (const char **) argv);
}
