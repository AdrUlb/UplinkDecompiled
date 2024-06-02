#include <Button.hpp>

void SetColour(const char* name);
void border_draw(Button* button);
void imagebutton_draw(Button* button, bool highlighted, bool clicked);
void textbutton_draw(Button* button, bool highlighted, bool clicked);
void button_draw(Button* button, bool highlighted, bool clicked);
void button_click(Button* button);
void clear_draw(int x, int y, int width, int height);
void button_highlight(Button* button);
void opengl_initialise();
void opengl_run();
void opengl_close();
void button_assignbitmap(const char* buttonName, const char* imageName);
void button_assignbitmaps(const char* buttonName, const char* normalFile, const char* highlightedFile, const char* clickedFile);
void imagebutton_drawtextured(Button* button, bool highlighted, bool clicked);
void create_msgbox(const char* title, const char* text, ButtonMouseUpFunc closeCallback);
void remove_msgbox();
