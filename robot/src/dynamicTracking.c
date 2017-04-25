#include "dynamicTracking.h"

/*!
* \brief Tell if the two rectangles are currently intersecting each other
* \param[in] rect1 : the first CvRect
* \param[in] rect2 : the second CvRect
* \return true if rectangles are currently intersecting each other, false otherwise
*/
boolean intersectRect(CvRect rect1, CvRect rect2)
{
	if(rect1.x<= rect2.x){
		if(rect2.x <= rect1.x + rect1.width){
			if(rect1.y<= rect2.y){
				if(rect2.y <= rect1.y + rect1.width{
					return true;
				} else {
					return false;
				}
			} else {
				if(rect1.y <= rect2.y + rect2.height){
					return true;
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
	} else {
		return intersectRect(rect2,rect1);
	}
}
