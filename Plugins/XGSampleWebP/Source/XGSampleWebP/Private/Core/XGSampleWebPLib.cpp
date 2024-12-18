#define _CRT_SECURE_NO_WARNINGS

#include "XGSampleWebPLib.h"

#if PLATFORM_WINDOWS
#include "mux.h"
#include "encode.h"
#include "decode.h"
#include "demux.h"
#endif


bool FXGSampleWebPLib::GenerateWebpByRGBA(const char* InWebpSavePath,
	const unsigned char* InRGBAData,
	int InWidth,
	int InHeight,
	float InQualityFactor)
{

#if PLATFORM_WINDOWS

	if (InQualityFactor < 0)
	{
		InQualityFactor = 0;
	}
	if (InQualityFactor > 100)
	{
		InQualityFactor = 100;
	}

	uint8_t* Output = nullptr;

	size_t   Size = WebPEncodeRGBA(InRGBAData, InWidth, InHeight, InWidth * 4, InQualityFactor, &Output);

	if (Size == 0)
	{
		return false;
	}

	FILE* FDes = fopen(InWebpSavePath, "wb");

	if (FDes)
	{
		fwrite(Output, Size, 1, FDes);
		fclose(FDes);

	}
	else
	{
		WebPFree(Output);
		return false;
	}

	WebPFree(Output);
	return true;


#else
	return false;
#endif


}

bool FXGSampleWebPLib::GenerateDymaicWebpByRGBA(const char* InWebpSavePath,
	std::vector<const unsigned char*>& InRGBADatas,
	std::vector<int> InTimestamps_ms,
	int InWidth,
	int InHeight,
	float InQualityFactor)
{
#if PLATFORM_WINDOWS

	if (InQualityFactor < 0)
	{
		InQualityFactor = 0;
	}
	if (InQualityFactor > 100)
	{
		InQualityFactor = 100;
	}

	if (InTimestamps_ms.size() != InRGBADatas.size())
	{
		return false;
	}


	WebPConfig WebpPictureConfig;

	if (!WebPConfigInit(&WebpPictureConfig))
	{
		return false;
	}


	WebPAnimEncoderOptions EncodeOptions;

	if (!WebPAnimEncoderOptionsInit(&EncodeOptions))
	{
		return false;
	}

	EncodeOptions.kmin = 0;
	EncodeOptions.kmax = 1;

	WebpPictureConfig.lossless = 0;
	WebpPictureConfig.method = 4;
	WebpPictureConfig.quality = InQualityFactor;



	WebPAnimEncoder* Encoder = WebPAnimEncoderNew(InWidth, InHeight, &EncodeOptions);


	float                                       TimePoint = 0.f;

	std::vector<const unsigned char*>::iterator DataItem = InRGBADatas.begin();
	std::vector<int>::iterator                  TimeStepItem = InTimestamps_ms.begin();

	for (; DataItem != InRGBADatas.end(); ++DataItem, ++TimeStepItem)
	{
		WebPPicture WebpOneFramePicture;
		WebPPictureInit(&WebpOneFramePicture);

		WebpOneFramePicture.use_argb = 1;
		WebpOneFramePicture.width = InWidth;
		WebpOneFramePicture.height = InHeight;
		WebpOneFramePicture.argb_stride = InWidth * 4;

		WebPPictureImportRGBA(&WebpOneFramePicture, *DataItem, InWidth * 4);

		TimePoint += *TimeStepItem;

		WebPAnimEncoderAdd(Encoder, &WebpOneFramePicture, TimePoint, &WebpPictureConfig);
		WebPPictureFree(&WebpOneFramePicture);

	}
	WebPAnimEncoderAdd(Encoder, NULL, TimePoint, NULL);

	WebPData WebpPictureData;
	WebPAnimEncoderAssemble(Encoder, &WebpPictureData);
	WebPAnimEncoderDelete(Encoder);

	FILE* FDes = fopen(InWebpSavePath, "wb");
	if (FDes)
	{
		fwrite(WebpPictureData.bytes, WebpPictureData.size, 1, FDes);
		fclose(FDes);

		WebPDataClear(&WebpPictureData);

		return true;
	}
	else
	{
		WebPDataClear(&WebpPictureData);
		return false;
	}






#else

	return false;


#endif


}

bool FXGSampleWebPLib::LoadDynamicWebpPictureByRGBA(const char* InWebpFilePath,
	std::vector<const unsigned char*>& OutRGBADatas,
	std::vector<int>& OutTimestamps_ms,
	int& OutWidth,
	int& OutHeight)
{
#if PLATFORM_WINDOWS
	bool bSucceed = true;

	FILE* FFileSource = fopen(InWebpFilePath, "rb");

	if (!FFileSource)
	{
		bSucceed = false;
		return bSucceed;
	}


	//简而言之就是在内存准备一段长度的空间,把磁盘上的文件写到这块空间
	unsigned char* FileOriginalData;
	unsigned int   FileLength;
	fseek(FFileSource, 0, SEEK_END);
	FileLength = ftell(FFileSource);
	FileOriginalData = (unsigned char*)WebPMalloc(FileLength);
	memset(FileOriginalData, 0, FileLength);
	fseek(FFileSource, 0, SEEK_SET);
	fread(FileOriginalData, FileLength, 1, FFileSource);
	fclose(FFileSource);


	WebPData               OriginWebpData = { FileOriginalData, (size_t)FileLength };

	WebPAnimDecoderOptions DecOptions;
	WebPAnimDecoderOptionsInit(&DecOptions);

	WebPAnimDecoder* Decoder = WebPAnimDecoderNew(&OriginWebpData, &DecOptions);
	WebPAnimInfo     AnimInfo;
	WebPAnimDecoderGetInfo(Decoder, &AnimInfo);

	OutWidth = AnimInfo.canvas_width;
	OutHeight = AnimInfo.canvas_height;

	OutRGBADatas.clear();
	OutTimestamps_ms.clear();

	int framNum = 0;
	int Timestamp = 0;

	while (WebPAnimDecoderHasMoreFrames(Decoder))
	{
		uint8_t* buf;

		WebPAnimDecoderGetNext(Decoder, &buf, &Timestamp);
		framNum++;

		unsigned char* oneWebpColorData = (unsigned char*)malloc(OutWidth * OutHeight * 4);

		memset(oneWebpColorData, 0, OutWidth * OutHeight * 4);
		memcpy(oneWebpColorData, buf, OutWidth * OutHeight * 4);

		OutRGBADatas.push_back(oneWebpColorData);
		OutTimestamps_ms.push_back(Timestamp);


	}
	WebPAnimDecoderReset(Decoder);
	WebPAnimDecoderDelete(Decoder);
	WebPDataClear(&OriginWebpData);

	return bSucceed;


#else

	return false;


#endif

}
