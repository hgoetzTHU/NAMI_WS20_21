#ifndef MITKCYLINDRICTOCARTESIANFILTER_H_HEADER_INCLUDED_C1F48A22
#define MITKCYLINDRICTOCARTESIANFILTER_H_HEADER_INCLUDED_C1F48A22

#include "mitkCommon.h"
#include "ImageToImageFilter.h"

#define M_PI       3.14159265358979323846
//#endif
namespace mitk {
//##Documentation
//## @brief Filter to convert a cylindric image into a cartesian image
//## @ingroup Process
//## @todo position of input image is not yet used to calculate position of the output
//## @todo tag with transducer position not yet transformed.
//## @todo the line limiting the sector is assumed to be available as a pic tag. Is this ok?
class CylindricToCartesianFilter : public ImageToImageFilter
{
public:
    mitkClassMacro(CylindricToCartesianFilter, ImageToImageFilter);

	itkNewMacro(Self);

    //##Documentation
    //## @brief Set background grey level
	itkSetMacro(OutsideValue, float);
    //##Documentation
    //## @brief Get background grey level
	itkGetMacro(OutsideValue, float);

    //##Documentation
    //## @brief Set the size in x-direction of the converted image.
	//## 
	//## A value of 0 (default) means that the filter uses the x-size of the input image.
	itkSetMacro(TargetXSize, unsigned int);
    //##Documentation
    //## @brief Get the size in x-direction of the converted image.
	itkGetMacro(TargetXSize, unsigned int);

	//##Documentation
	//## @brief Build some tables to speed up transformation, e.g., to avoid repeated calulations of the same sinus/cosinus.
	//## @todo Free tables if they were already previously allocated
    void buildTransformShortCuts(int orig_xsize, int orig_ysize, int orig_zsize, int new_xsize, ipPicDescriptor * &rt_pic, ipPicDescriptor * &phit_pic, ipPicDescriptor * &fr_pic, ipPicDescriptor * &fphi_pic, unsigned int * &zt, float * &fz);

	//##Documentation
	//## @brief Build some tables to speed up transformation of images that only contain data within a sector
	//## @todo Free tables if they were already previously allocated
	void buildConeCutOffShortCut(int orig_xsize, int orig_ysize, ipPicDescriptor *rt_pic, ipPicDescriptor *fr_pic, float a, float b, ipPicDescriptor * &coneCutOff_pic);
protected:
	ipPicDescriptor * rt_pic, * phit_pic, *fr_pic, * fphi_pic, * coneCutOff_pic;
	float * fz;
	unsigned int * zt;
    //##Description 
    //## @brief Cut off line for images that only contain data within a sector
	//## 
	//## y=a x+b
	float a;
    //##Description 
    //## @brief Cut off line for images that only contain data within a sector
	//## 
	//## y=a x+b
	float b;

    //##Description 
    //## @brief Time when Header was last initialized
    itk::TimeStamp m_TimeOfHeaderInitialization;
  protected:
    virtual void GenerateData();

	virtual void GenerateOutputInformation();

	virtual void GenerateInputRequestedRegion();

	CylindricToCartesianFilter();

    ~CylindricToCartesianFilter();

	float m_OutsideValue;
	unsigned int m_TargetXSize;
};

} // namespace mitk

#endif /* MITKCYLINDRICTOCARTESIANFILTER_H_HEADER_INCLUDED_C1F48A22 */

