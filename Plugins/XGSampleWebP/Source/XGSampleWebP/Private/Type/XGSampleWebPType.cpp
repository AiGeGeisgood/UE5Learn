#include "XGSampleWebPType.h"


FXGSampleWebpPictureInformation::FXGSampleWebpPictureInformation() :
	X0(0),
	Y0(0),
	X1(0),
	Y1(0)
{

}

FXGSampleWebpPictureInformation::FXGSampleWebpPictureInformation(FXGSampleWebpPictureInformation& InWebpPictureInformation) :
	X0(InWebpPictureInformation.X0),
	Y0(InWebpPictureInformation.Y0),
	X1(InWebpPictureInformation.X1),
	Y1(InWebpPictureInformation.Y1)
{

}

FXGSampleWebpPictureInformation::FXGSampleWebpPictureInformation(int32 InX0, int32 InY0, int32 InX1, int32 InY1) :
	X0(InX0),
	Y0(InY0),
	X1(InX1),
	Y1(InY1)
{

}

int32 FXGSampleWebpPictureInformation::GetPictureWidth()
{
	return X1 - X0 + 1 > 0 ? X1 - X0 + 1 : 0;

}
//(0,0)  (1919,1079)
int32 FXGSampleWebpPictureInformation::GetPictureHeight()
{
	return Y1 - Y0 + 1 > 0 ? Y1 - Y0 + 1 : 0;

}

void FXGSampleWebpPictureInformation::ResetPictureInformation()
{
	X0 = 0;
	X1 = 0;
	Y0 = 0;
	Y1 = 0;
}
