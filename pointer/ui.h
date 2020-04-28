
// These methods are provided by the main code
extern int uiGetSectionCount();
extern const char* uiGetSectionFromIndex(int sectionIndex);
extern int uiGetOptionCount(int sectionIndex);
extern const char* uiGetOptionFromIndex(int sectionIndex, int optionIndex);
extern void uiInvokeOption(int, int);

// A simple interface, with left/right buttons and select
// Select activates the option on the current row, and switches
// to the other.
class TwoRowUI {
public:
    TwoRowUI(Debounce& left, Debounce& right, Debounce& select)
        : keyLeft(left)
        , keyRight(right)
        , keySelect(select)
    {
        // nop
    }

    void setup()
    {
        sectionIndex = 0;
        row = 0;
    }

    bool update()
    {
        if (keyLeft.update() && keyLeft.getState()) {
            if (row == 0) {
                if (sectionIndex == 0) {
                    sectionIndex = uiGetSectionCount();
                }
                --sectionIndex;
            }
            else { // command
                if (optionIndex == 0) {
                    optionIndex = uiGetOptionCount(sectionIndex);
                }
                --optionIndex;
            }
            return true;
        }
        //
        if (keyRight.update() && keyRight.getState()) {
            if (row == 0) {
                if (++sectionIndex == uiGetSectionCount()) {
                    sectionIndex = 0;
                }
            }
            else {
                if (++optionIndex == uiGetOptionCount(sectionIndex)) {
                    optionIndex = 0;
                }
            }
            return true;
        }
        //
        if (keySelect.update() && keySelect.getState()) {
            if (row == 0) {
                row = 1;
            }
            else {
                uiInvokeOption(sectionIndex, optionIndex);
                row = 0;
            }
            return true;
        }
        return false;
    }

private:
    Debounce &keyLeft, &keyRight, &keySelect;

public:
    int sectionIndex;
    int optionIndex;
    int row;
};
