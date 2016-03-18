//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.01.0784
// ----------------------------------------------------------------------------
// Standard GradientDomain Process Module Version 00.06.04.0165
// ----------------------------------------------------------------------------
// GradientsHdrCompositionInstance.h - Released 2016/02/21 20:22:43 UTC
// ----------------------------------------------------------------------------
// This file is part of the standard GradientDomain PixInsight module.
//
// Copyright (c) Georg Viehoever, 2011-2015. Licensed under LGPL 2.1
// Copyright (c) 2003-2015 Pleiades Astrophoto S.L.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
// ----------------------------------------------------------------------------

#ifndef __GradientsHdrCompositionInstance_h
#define __GradientsHdrCompositionInstance_h

#include <pcl/ProcessImplementation.h>
#include <pcl/Vector.h>

#include "GradientsHdrComposition.h" //for image types

namespace pcl
{

  class GradientsHdrCompositionInstance : public ProcessImplementation
  {
  public:

    GradientsHdrCompositionInstance( const MetaProcess* );
    GradientsHdrCompositionInstance( const GradientsHdrCompositionInstance& );

    virtual void Assign( const ProcessImplementation& );

    virtual bool CanExecuteOn( const View&, String& whyNot ) const;
    virtual bool IsHistoryUpdater( const View& v ) const;

    virtual bool CanExecuteGlobal( String& whyNot ) const;
    virtual bool ExecuteGlobal();

    virtual void* LockParameter( const MetaParameter*, size_type /*tableRow*/ );
    virtual bool AllocateParameter( size_type sizeOrLength, const MetaParameter* p, size_type tableRow );
    virtual size_type ParameterLength( const MetaParameter* p, size_type tableRow ) const;


  private: //functions
    /// create image window that has rImage as contents
    static ImageWindow CreateImageWindow( const IsoString& id, GradientsHdrComposition::imageType_t const &rImage_p);

    /// create image window that has rImage as contents
    static ImageWindow CreateImageWindow( const IsoString& id, GradientsHdrComposition::numImageType_t const &rImage_p);

  private: //slots
    struct ImageItem
    {
      pcl_bool enabled; // if disabled, skip (ignore) this image
      String   path;    // absolute file path

    ImageItem( const String& p = String() ) : enabled( true ), path( p )
      {
      }

    ImageItem( const ImageItem& x ) : enabled( x.enabled ), path( x.path )
      {
      }

      bool IsValid() const
      {
	return !enabled || !path.IsEmpty();
      }
    };

    typedef Array<ImageItem>  image_list;

    /// The set of target frames to be calibrated
    image_list      targetFrames;

    /// log10(offset) of true black
    double dLogBias;

    /// true if result should be in log space (better for visualization). Otherwise try to get linear result
    pcl_bool bKeepLog;

    /// true if bias is negative
    pcl_bool bNegativeBias;

    /// true if we need to generate a mask view
    pcl_bool generateMask;

    friend class GradientsHdrCompositionInterface;
  };

  // ----------------------------------------------------------------------------

} // pcl

#endif

// ----------------------------------------------------------------------------
// EOF GradientsHdrCompositionInstance.h - Released 2016/02/21 20:22:43 UTC
