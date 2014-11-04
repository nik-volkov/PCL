// ****************************************************************************
// PixInsight Class Library - PCL 02.00.13.0689
// ****************************************************************************
// pcl/SpinBox.cpp - Released 2014/10/29 07:34:20 UTC
// ****************************************************************************
// This file is part of the PixInsight Class Library (PCL).
// PCL is a multiplatform C++ framework for development of PixInsight modules.
//
// Copyright (c) 2003-2014, Pleiades Astrophoto S.L. All Rights Reserved.
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
// ****************************************************************************

#include <pcl/SpinBox.h>
#include <pcl/TextAlign.h>

#include <pcl/api/APIException.h>
#include <pcl/api/APIInterface.h>

namespace pcl
{

// ----------------------------------------------------------------------------

#define sender    (reinterpret_cast<SpinBox*>( hSender ))
#define receiver  (reinterpret_cast<Control*>( hReceiver ))

class SpinBoxEventDispatcher
{
public:

   static void api_func ValueUpdated( control_handle hSender, control_handle hReceiver, int32 value )
   {
      if ( sender->onValueUpdated != 0 )
         (receiver->*sender->onValueUpdated)( *sender, value );
   }

}; // SpinBoxEventDispatcher

#undef sender
#undef receiver

// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#  pragma warning( disable: 4355 ) // 'this' : used in base member initializer list
#endif

/*
 * ### TODO: Create a new class to encapsulate the common behavior of SpinBox
 *           and Slider. Proposal of class name: RangeControl
 */

SpinBox::SpinBox( Control& parent ) :
Control( (*API->SpinBox->CreateSpinBox)( ModuleHandle(), this, parent.handle, 0 /*flags*/ ) ),
onValueUpdated( 0 )
{
   if ( handle == 0 )
      throw APIFunctionError( "CreateSpinBox" );
}

// ----------------------------------------------------------------------------

void SpinBox::OnValueUpdated( value_event_handler f, Control& receiver )
{
   __PCL_NO_ALIAS_HANDLER;
   onValueUpdated = 0;
   if ( (*API->SpinBox->SetSpinBoxValueUpdatedEventRoutine)( handle, &receiver,
        (f != 0) ? SpinBoxEventDispatcher::ValueUpdated : 0 ) == api_false )
   {
      throw APIFunctionError( "SetSpinBoxValueUpdatedEventRoutine" );
   }
   onValueUpdated = f;
}

// ----------------------------------------------------------------------------

int SpinBox::Value() const
{
   return (*API->SpinBox->GetSpinBoxValue)( handle );
}

// ----------------------------------------------------------------------------

void SpinBox::SetValue( int value )
{
   return (*API->SpinBox->SetSpinBoxValue)( handle, value );
}

// ----------------------------------------------------------------------------

double SpinBox::NormalizedValue() const
{
   int v0, v1; GetRange( v0, v1 );
   return (v0 != v1) ? double( Value() - v0 )/(v1 - v0) : 0.0;
}

// ----------------------------------------------------------------------------

void SpinBox::SetNormalizedValue( double f )
{
   int v0, v1; GetRange( v0, v1 );
   SetValue( v0 + RoundI( Range( f, 0.0, 1.0 )*(v1 - v0) ) );
}

// ----------------------------------------------------------------------------

void SpinBox::GetRange( int& minValue, int& maxValue ) const
{
   (*API->SpinBox->GetSpinBoxRange)( handle, &minValue, &maxValue );
}

// ----------------------------------------------------------------------------

void SpinBox::SetRange( int minValue, int maxValue )
{
   (*API->SpinBox->SetSpinBoxRange)( handle, minValue, maxValue );
}

// ----------------------------------------------------------------------------

int SpinBox::StepSize() const
{
   return (*API->SpinBox->GetSpinBoxStepSize)( handle );
}

// ----------------------------------------------------------------------------

void SpinBox::SetStepSize( int size )
{
   (*API->SpinBox->SetSpinBoxStepSize)( handle, size );
}

// ----------------------------------------------------------------------------

bool SpinBox::IsWrappingEnabled() const
{
   return (*API->SpinBox->GetSpinBoxWrappingEnabled)( handle ) != api_false;
}

// ----------------------------------------------------------------------------

void SpinBox::EnableWrapping( bool enable )
{
   (*API->SpinBox->SetSpinBoxWrappingEnabled)( handle, enable );
}

// ----------------------------------------------------------------------------

bool SpinBox::IsEditable() const
{
   return (*API->SpinBox->GetSpinBoxEditable)( handle ) != api_false;
}

// ----------------------------------------------------------------------------

void SpinBox::SetEditable( bool enable )
{
   (*API->SpinBox->SetSpinBoxEditable)( handle, enable );
}

// ----------------------------------------------------------------------------

String SpinBox::Prefix() const
{
   size_type len = 0;
   (*API->SpinBox->GetSpinBoxPrefix)( handle, 0, &len );

   String prefix;

   if ( len != 0 )
   {
      prefix.Reserve( len );

      if ( (*API->SpinBox->GetSpinBoxPrefix)( handle, prefix.c_str(), &len ) == api_false )
         throw APIFunctionError( "GetSpinBoxPrefix" );
   }

   return prefix;
}

// ----------------------------------------------------------------------------

void SpinBox::SetPrefix( const String& txt )
{
   (*API->SpinBox->SetSpinBoxPrefix)( handle, txt.c_str() );
}

// ----------------------------------------------------------------------------

String SpinBox::Suffix() const
{
   size_type len = 0;
   (*API->SpinBox->GetSpinBoxSuffix)( handle, 0, &len );

   String suffix;

   if ( len != 0 )
   {
      suffix.Reserve( len );

      if ( (*API->SpinBox->GetSpinBoxSuffix)( handle, suffix.c_str(), &len ) == api_false )
         throw APIFunctionError( "GetSpinBoxSuffix" );
   }

   return suffix;
}

// ----------------------------------------------------------------------------

void SpinBox::SetSuffix( const String& txt )
{
   (*API->SpinBox->SetSpinBoxSuffix)( handle, txt.c_str() );
}

// ----------------------------------------------------------------------------

String SpinBox::MinimumValueText() const
{
   size_type len = 0;
   (*API->SpinBox->GetSpinBoxMinimumValueText)( handle, 0, &len );

   String text;

   if ( len != 0 )
   {
      text.Reserve( len );

      if ( (*API->SpinBox->GetSpinBoxMinimumValueText)( handle, text.c_str(), &len ) == api_false )
         throw APIFunctionError( "GetSpinBoxMinimumValueText" );
   }

   return text;
}

// ----------------------------------------------------------------------------

void SpinBox::SetMinimumValueText( const String& txt )
{
   (*API->SpinBox->SetSpinBoxMinimumValueText)( handle, txt.c_str() );
}

// ----------------------------------------------------------------------------

bool SpinBox::IsRightAligned() const
{
   return (*API->SpinBox->GetSpinBoxAlignment)( handle ) == TextAlign::Right;
}

// ----------------------------------------------------------------------------

void SpinBox::SetRightAlignment( bool right )
{
   (*API->SpinBox->SetSpinBoxAlignment)( handle, right ? TextAlign::Right : TextAlign::Left );
}

// ----------------------------------------------------------------------------

} // pcl

// ****************************************************************************
// EOF pcl/SpinBox.cpp - Released 2014/10/29 07:34:20 UTC