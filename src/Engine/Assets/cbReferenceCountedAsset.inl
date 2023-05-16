#ifndef GE3D_ASSETS_CBREFERENCECOUNTEDASSET_INL
#define GE3D_ASSETS_CBREFERENCECOUNTEDASSET_INL

// Include Files
//==============

#include "cbReferenceCountedAsset.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

#ifdef GE3D_ASSETS_SHOULDREFERENCECOUNTBECHECKEDINDESTRUCTOR

	// Implementation
	//===============

	// Initialization / Clean Up
	//--------------------------

	template <class tAsset>
		inline GE3D::Assets::cbReferenceCountedAsset<tAsset>::~cbReferenceCountedAsset()
	{
		GE3D_ASSERTF( m_referenceCount == 0, "A reference counted asset is being deleted with a non-zero reference count (%u)", m_referenceCount );
		GE3D::Logging::OutputError( "A reference counted asset was destructed with a non-zero reference count (%u)", m_referenceCount );
	}

#endif	// GE3D_ASSETS_SHOULDREFERENCECOUNTBECHECKEDINDESTRUCTOR

#if defined( GE3D_PLATFORM_WINDOWS )
	#include "Windows/cbReferenceCountedAsset.win.inl"
#endif

#endif	// GE3D_ASSETS_CBREFERENCECOUNTEDASSET_INL
