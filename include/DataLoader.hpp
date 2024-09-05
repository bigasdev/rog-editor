#include <iostream>
#include <SDL.h>              
#include <tinyfiledialogs.h>

class Data_Loader {
public:
    // load a file with specific extension
    static const char* load_file(const char *filter) {
        const char *lFilterPatterns[1] = {filter};
        const char *file = tinyfd_openFileDialog("Open File", NULL, 1, lFilterPatterns, NULL, 0);

        if (!file) {
            return "";
        }
        return file;
    }

    // load a folder with a title
    static const char* load_folder(const char *title) {
        const char *folder = tinyfd_selectFolderDialog(title, SDL_GetBasePath());

        if (!folder) {
            return "";
        }
        return folder;
    }

    // save a file with specific extension
    static const char* save_file(const char *filter) {
        const char *lFilterPatterns[1] = {filter};
        const char *file = tinyfd_saveFileDialog("Save File", NULL, 1, lFilterPatterns, NULL);

        if (!file) {
            return "";
        }
        return file;
    }
};
