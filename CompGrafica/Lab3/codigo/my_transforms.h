#ifndef MY_TRANSFORMS
#define	MY_TRANSFORMS

void mSetIdentity();
void mPrintMTransform();

void mTranslate(float dx, float dy, float dz);
void mRotate(float thetaX, float thetaY, float thetaZ);

void mApplyToPoint(float *x, float *y, float *z);


#endif	/* MY_TRANSFORMS */