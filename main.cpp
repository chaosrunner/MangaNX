//headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <switch.h>


// Validates Keyboard text 
SwkbdTextCheckResult validate_text(char* tmp_string, size_t tmp_string_size) {
    if (strncmp(tmp_string, "https://", 8) !=0 && strncmp(tmp_string, "http://", 7) !=0) {
        strncpy(tmp_string, "Add 'https://' or 'http://' to url", tmp_string_size);
        return SwkbdTextCheckResult_Bad;
    }    
    return SwkbdTextCheckResult_OK;
}
// Keyboard
int showKeyboard(char out[0xc00], char* title, char* placehold, char* oktxt, char* initial)
{
    int rc;
    SwkbdConfig kbd;
    char tmpoutstr[0xc00] = {0};
    rc = swkbdCreate(&kbd, 0);
    if(R_SUCCEEDED(rc)) {
        swkbdConfigMakePresetDefault(&kbd);
        swkbdConfigSetHeaderText(&kbd, title);
        swkbdConfigSetOkButtonText(&kbd, oktxt);
        swkbdConfigSetGuideText(&kbd, placehold);
        swkbdConfigSetInitialText(&kbd, initial);
        swkbdConfigSetTextCheckCallback(&kbd, validate_text);
        rc = swkbdShow(&kbd, tmpoutstr, sizeof(tmpoutstr));
        if(R_SUCCEEDED(rc)) {
            if(tmpoutstr[0] != '\0') {
                strcpy(out, tmpoutstr);
                swkbdClose(&kbd);
                return 0;
            }
        }
    }
    swkbdClose(&kbd);
    return -1;
}

int main(int argc, char* argv[])
{
    // initilizeing variables 
    char url[0xc00] = {0};
    consoleInit(NULL);
    printf("KIMOCHIIINX\n\nPress + to quit.\n");
    printf(CONSOLE_BLUE "Press X to open the Search Bar\n");
    if (!(appletGetAppletType() == AppletType_Application)) {
        printf(CONSOLE_RED "Currently Running in applet mode Browser WILL NOT WORK, exit and launch via application mode .\n");
    }

    // Main loop
    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        // Press Key to Execute 
        if (kDown & KEY_PLUS)
            break;
        if (kDown & KEY_X)
            {
            showKeyboard(url, "Choose URL", "Enter a URL", "Go", "https://mangadex.org/");
             Result rc;
             WebCommonConfig config;
            // Creating the config.
            rc = webPageCreate(&config, url);
            if (R_SUCCEEDED(rc)) {
            rc = webConfigSetWhitelist(&config, "^http*");
            if (R_SUCCEEDED(rc)) { 
            rc = webConfigShow(&config, NULL);
                }
            }
        }
        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    return 0;
}










































































