/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkPhotoacousticImage.h"
#include "mitkUSDiPhASBModeImageFilter.h"
#include "Algorithms/ITKUltrasound/itkBModeImageFilter.h"
#include "Algorithms/itkPhotoacousticBModeImageFilter.h"
#include "mitkImageCast.h"
#include "mitkITKImageImport.h"

// itk dependencies
#include "itkImage.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCropImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkIntensityWindowingImageFilter.h"
#include <itkIndex.h>
#include "itkMultiplyImageFilter.h"

mitk::PhotoacousticImage::PhotoacousticImage()
{
  MITK_INFO << "[PhotoacousticImage Debug] created that image";
}

mitk::PhotoacousticImage::~PhotoacousticImage()
{
  MITK_INFO << "[PhotoacousticImage Debug] destroyed that image";
}

mitk::Image::Pointer mitk::PhotoacousticImage::ApplyBmodeFilter(mitk::Image::Pointer inputImage, bool UseLogFilter, float resampleSpacing)
{
  // we use this seperate ApplyBmodeFilter Method for processing of two-dimensional images

  // the image needs to be of floating point type for the envelope filter to work; the casting is done automatically by the CastToItkImage
  typedef itk::Image< float, 3 > itkFloatImageType;

  typedef itk::BModeImageFilter < itkFloatImageType, itkFloatImageType > BModeFilterType;
  BModeFilterType::Pointer bModeFilter = BModeFilterType::New();  // LogFilter

  typedef itk::PhotoacousticBModeImageFilter < itkFloatImageType, itkFloatImageType > PhotoacousticBModeImageFilter;
  PhotoacousticBModeImageFilter::Pointer photoacousticBModeFilter = PhotoacousticBModeImageFilter::New(); // No LogFilter

  typedef itk::ResampleImageFilter < itkFloatImageType, itkFloatImageType > ResampleImageFilter;
  ResampleImageFilter::Pointer resampleImageFilter = ResampleImageFilter::New();

  itkFloatImageType::Pointer itkImage;

  mitk::CastToItkImage(inputImage, itkImage);

  itkFloatImageType::Pointer bmode;

  if (UseLogFilter)
  {
    bModeFilter->SetInput(itkImage);
    bModeFilter->SetDirection(1);
    bmode = bModeFilter->GetOutput();
  }
  else
  {
    photoacousticBModeFilter->SetInput(itkImage);
    photoacousticBModeFilter->SetDirection(1);
    bmode = photoacousticBModeFilter->GetOutput();
  }

  // resampleSpacing == 0 means: do no resampling
  if (resampleSpacing == 0)
  {
    return mitk::GrabItkImageMemory(bmode);
  }

  itkFloatImageType::SpacingType outputSpacing;
  itkFloatImageType::SizeType inputSize = itkImage->GetLargestPossibleRegion().GetSize();
  itkFloatImageType::SizeType outputSize = inputSize;
  outputSize[0] = 256;

  outputSpacing[0] = itkImage->GetSpacing()[0] * (static_cast<double>(inputSize[0]) / static_cast<double>(outputSize[0]));
  outputSpacing[1] = resampleSpacing;
  outputSpacing[2] = 0.6;

  outputSize[1] = inputSize[1] * itkImage->GetSpacing()[1] / outputSpacing[1];

  typedef itk::IdentityTransform<double,3> TransformType;
  resampleImageFilter->SetInput(bmode);
  resampleImageFilter->SetSize(outputSize);
  resampleImageFilter->SetOutputSpacing(outputSpacing);
  resampleImageFilter->SetTransform(TransformType::New());

  resampleImageFilter->UpdateLargestPossibleRegion();
  return mitk::GrabItkImageMemory(resampleImageFilter->GetOutput());
}

//mitk::Image::Pointer mitk::PhotoacousticImage::ApplyScatteringCompensation(mitk::Image::Pointer inputImage, int scattering)
//{
//  typedef itk::Image< float, 3 > itkFloatImageType;
//  typedef itk::MultiplyImageFilter <itkFloatImageType, itkFloatImageType > MultiplyImageFilterType;

//  itkFloatImageType::Pointer itkImage;
//  mitk::CastToItkImage(inputImage, itkImage);

//  MultiplyImageFilterType::Pointer multiplyFilter = MultiplyImageFilterType::New();
//  multiplyFilter->SetInput1(itkImage);
//  multiplyFilter->SetInput2(m_FluenceCompResizedItk.at(m_ScatteringCoefficient));

//  return mitk::GrabItkImageMemory(multiplyFilter->GetOutput());
//}

mitk::Image::Pointer mitk::PhotoacousticImage::ApplyResampling(mitk::Image::Pointer inputImage, mitk::Vector3D outputSpacing, unsigned int outputSize[3])
{
  typedef itk::Image< double, 3 > itkFloatImageType;

  typedef itk::ResampleImageFilter < itkFloatImageType, itkFloatImageType > ResampleImageFilter;
  ResampleImageFilter::Pointer resampleImageFilter = ResampleImageFilter::New();

  itkFloatImageType::Pointer itkImage;

  mitk::CastToItkImage(inputImage, itkImage);


  itkFloatImageType::SpacingType outputSpacingItk;
  itkFloatImageType::SizeType inputSizeItk = itkImage->GetLargestPossibleRegion().GetSize();
  itkFloatImageType::SizeType outputSizeItk = inputSizeItk;
  itkFloatImageType::SpacingType inputSpacing = itkImage->GetSpacing();

  outputSizeItk[0] = outputSize[0];
  outputSizeItk[1] = (inputSizeItk[1] * outputSize[0] / inputSizeItk[1]);
  outputSizeItk[2] = 1;

  outputSpacingItk[0] = inputSpacing[0] * (double)inputSizeItk[0] / (double)outputSize[0];
  outputSpacingItk[1] = inputSpacing[1] * (double)inputSizeItk[1] / (double)outputSize[1];
  outputSpacingItk[2] = outputSpacing[2];

  typedef itk::IdentityTransform<double, 3> TransformType;
  resampleImageFilter->SetInput(itkImage);
  resampleImageFilter->SetSize(outputSizeItk);
  resampleImageFilter->SetOutputSpacing(outputSpacingItk);
  resampleImageFilter->SetTransform(TransformType::New());

  resampleImageFilter->UpdateLargestPossibleRegion();
  return mitk::GrabItkImageMemory(resampleImageFilter->GetOutput());
}
