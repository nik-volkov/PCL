//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.01.0784
// ----------------------------------------------------------------------------
// Standard INDIClient Process Module Version 01.00.03.0102
// ----------------------------------------------------------------------------
// INDICCDFrameInstance.h - Released 2016/03/18 13:15:37 UTC
// ----------------------------------------------------------------------------
// This file is part of the standard INDIClient PixInsight module.
//
// Copyright (c) 2014-2016 Klaus Kretzschmar
//
// Redistribution and use in both source and binary forms, with or without
// modification, is permitted provided that the following conditions are met:
//
// 1. All redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. All redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Neither the names "PixInsight" and "Pleiades Astrophoto", nor the names
//    of their contributors, may be used to endorse or promote products derived
//    from this software without specific prior written permission. For written
//    permission, please contact info@pixinsight.com.
//
// 4. All products derived from this software, in any form whatsoever, must
//    reproduce the following acknowledgment in the end-user documentation
//    and/or other materials provided with the product:
//
//    "This product is based on software from the PixInsight project, developed
//    by Pleiades Astrophoto and its contributors (http://pixinsight.com/)."
//
//    Alternatively, if that is where third-party acknowledgments normally
//    appear, this acknowledgment must be reproduced in the product itself.
//
// THIS SOFTWARE IS PROVIDED BY PLEIADES ASTROPHOTO AND ITS CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL PLEIADES ASTROPHOTO OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, BUSINESS
// INTERRUPTION; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; AND LOSS OF USE,
// DATA OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

#ifndef __INDICCDFrameInstance_h
#define __INDICCDFrameInstance_h

#include <pcl/MetaParameter.h>
#include <pcl/ProcessImplementation.h>

namespace pcl
{

// ----------------------------------------------------------------------------

class INDICCDFrameInstance : public ProcessImplementation
{
public:

   INDICCDFrameInstance( const MetaProcess* );
   INDICCDFrameInstance( const INDICCDFrameInstance& );

   virtual void Assign( const ProcessImplementation& );

   virtual bool CanExecuteOn( const View&, String& whyNot ) const;
   virtual bool CanExecuteGlobal( String& whyNot ) const;

   virtual bool ExecuteGlobal();

   virtual void* LockParameter( const MetaParameter*, size_type tableRow );
   virtual bool AllocateParameter( size_type sizeOrLength, const MetaParameter*, size_type tableRow );
   virtual size_type ParameterLength( const MetaParameter*, size_type tableRow ) const;

   bool ValidateDevice( bool throwErrors = true ) const;

   void SendDeviceProperties( bool asynchronous = true ) const;

   String ServerFileName( const String& fileNameTemplate ) const;

   String ServerFileName() const
   {
      return ServerFileName( p_serverFileNameTemplate );
   }

   static String UploadModePropertyString( int uploadModeIdx );
   static String CCDFrameTypePropertyString( int frameTypeIdx );
   static String CCDFrameTypePrefix( int frameTypeIdx );

private:

   String   p_deviceName;
   pcl_enum p_uploadMode;
   String   p_serverUploadDirectory;
   String   p_serverFileNameTemplate;
   pcl_enum p_frameType;
   int32    p_binningX;
   int32    p_binningY;
   double   p_exposureTime;
   double   p_exposureDelay;
   int32    p_exposureCount;
   String   p_newImageIdTemplate;

   int      m_exposureNumber;

   friend class INDICCDFrameInterface;
};

// ----------------------------------------------------------------------------


} // pcl

#endif   // __INDICCDFrameInstance_h

// ----------------------------------------------------------------------------
// EOF INDICCDFrameInstance.h - Released 2016/03/18 13:15:37 UTC
