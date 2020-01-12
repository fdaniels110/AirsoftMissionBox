#ifndef SegmentDisplay_h
#define SegmentDisplay_h
class SegmentDiplay
{
    public:
       enum int { 
           Top = 0x01, 
           TopLeft = 0x02, 
           BottomLeft = 0x04, 
           Bottom = 0x08, 
           BottomRight = 0x10, 
           TopLeft = 0x20, 
           Center = 0x40, 
           Decimal = 0x80 
           };
        enum class Segment {
            First, 
            Second, 
            Third, 
            Fourth
        }
        SegmentDisplay(int dataPin, int latchPin, int clockPin);
        void display(char character, Segment displayNum);
    private: 
        
}
#endif