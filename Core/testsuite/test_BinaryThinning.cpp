/*
* Copyright (c) 2018 The Foundation for Research on Information Technologies in Society (IT'IS).
*
* This file is part of iSEG
* (see https://github.com/ITISFoundation/osparc-iseg).
*
* This software is released under the MIT License.
*  https://opensource.org/licenses/MIT
*/
#include <boost/test/unit_test.hpp>

#include "../BinaryThinningImageFilter.h"
#include "../ImageConnectivtyGraph.h"

#define ENABLE_DUMP_IMAGE
#include "Data/ItkUtils.h"

namespace iseg {

BOOST_AUTO_TEST_SUITE(iSeg_suite);
BOOST_AUTO_TEST_SUITE(BinaryThinning_suite);

// TestRunner.exe --run_test=iSeg_suite/BinaryThinning_suite/BinaryThinning_test --log_level=message
BOOST_AUTO_TEST_CASE(BinaryThinning_test)
{
	using input_type = itk::Image<float, 3>;
	using output_type = itk::Image<unsigned short, 3>;

	itk::Index<3> start = {0, 0, 0};
	itk::Size<3> size = {50, 70, 45};

	auto input = input_type::New();
	input->SetRegions(itk::ImageRegion<3>(start, size));
	input->Allocate();
	input->FillBuffer(0.f);

	itk::Index<3> start2 = {10, 10, 10};
	itk::Size<3> size2 = {5, 50, 5};

	itk::ImageRegionIterator<input_type> it(input, itk::ImageRegion<3>(start2, size2));
	for (it.GoToBegin(); !it.IsAtEnd(); ++it)
	{
		it.Set(1.f);
	}

	auto thinning_filter = itk::MedialAxisImageFilter<input_type, output_type>::New();
	thinning_filter->SetInput(input);
	thinning_filter->Update();

	//dump_image(thinning_filter->GetOutput(), "E:/temp/thinned.mha");
}

// TestRunner.exe --run_test=iSeg_suite/BinaryThinning_suite/ImageConnectivityGraph_test --log_level=message
BOOST_AUTO_TEST_CASE(ImageConnectivityGraph_test)
{
	using image_type = itk::Image<unsigned char, 3>;
	itk::Index<3> start = {0, 0, 0};
	itk::Size<3> size = {50, 70, 45};

	auto input = image_type::New();
	input->SetRegions(itk::ImageRegion<3>(start, size));
	input->Allocate();
	input->FillBuffer(0);

	itk::Index<3> idx = {9, 10, 8};
	input->SetPixel(idx, 1);

	idx[1]++;
	input->SetPixel(idx, 1);

	idx[2]++;
	input->SetPixel(idx, 1);

	auto edges = ImageConnectivityGraph<image_type>(input, input->GetBufferedRegion());

	// here I understand why I get 3 and must do post-processing to get 2!
	BOOST_CHECK_EQUAL(edges.aligned_edges.size(), 2);
	BOOST_CHECK_EQUAL(edges.diag_edges.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE_END();

} // namespace iseg
