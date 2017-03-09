#include "view.h"

/*!
* \brief Color a rectangle
* \param[out] surface the drawing target
* \param[in] x the abscissa of the rectangle
* \param[in] y the ordinate of the rectangle
* \param[in] w the width of the rectangle
* \param[in] h the height of the rectangle
* \param[in] r the red component (integer between 0 and 255)
* \param[in] g the green component (integer between 0 and 255)
* \param[in] b the blue component (integer between 0 and 255)
*/
void drawRectangle(SDL_Surface * surface, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, r, g, b));
}
