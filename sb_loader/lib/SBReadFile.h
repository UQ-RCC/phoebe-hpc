/* III::SBReadFile provides read access to SlideBook .sld files.
 *
 * Changelog:
 *
 */

/** \mainpage SBReadFile
 * 
 * \section Usage Usage
 * Allocate an instance with III_NewSBReadFile(). See III::SBReadFile for usage of
 * an instance. Deallocate an instance with III_DeleteSBReadFile().
 *
 * Failure during allocation, opening and usage of an III::SBReadFile instance
 * are governed by an exception propogation mask. Some failure cases also
 * have an extended error code that can be provided to III_GetErrorString()
 * for a more detailed description of the failure.
 *
 * III_SBReadFileVersion( III_Version* outVersion )
 *
 * \subsection Allocate 
 * III_NewSBReadFile( const char* inFilepath, UInt32 inExceptionMask)
 *
 * \copydoc III_NewSBReadFile()
 *
 * \subsection Deallocate 
 * III_DeleteSBReadFile(III::SBReadFile*)
 *
 * \copydoc III_DeleteSBReadFile()
 *
 * \subsection FailurePolicy 
 *
 * \size_t III_GetErrorString( char* ioErrorString, 
 *                             const UInt32 inRdState, 
 *                             const UInt32 inErrorCode);
 *
 * \copydoc III_GetErrorString()
 */

#pragma once

#include <cstddef>

// a copy of shared/Types.h without the Win/Mac cross types.
// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system#UNIX
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#ifndef __unix__
#define __unix__
#endif
#endif

// __has_feature compatability thunk
// For all versions of Microsoft VC the required features exist.
#ifndef __has_feature
  #define __has_feature(x) 0
#endif

// Prefixed to, hopefully, not pollute the client preprocessor's macro
// namespace.
#ifdef _WIN32
#define III_EXPORT __declspec(dllexport)
#endif

// Use GCC visibility pragma to export all symbols declared in this file.
// No need for a III_EXPORT definition
#ifdef __unix__
#pragma GCC visibility push(default)
#define III_EXPORT 
#endif

// III types exported to the client
#ifndef __III_BASIC_TYPES__
#define __III_BASIC_TYPES__

#ifdef _WIN32

typedef unsigned char	UInt8;   
typedef signed char		SInt8;   
typedef unsigned short	UInt16;  
typedef short			SInt16;  

typedef unsigned long	UInt32;  
typedef long			SInt32;  
typedef unsigned long long		UInt64;  
typedef long long		SInt64;  

#endif // _WIN32

#ifdef __unix__

#include <limits.h>
#include <stdint.h>

typedef uint8_t	    UInt8;
typedef int8_t		SInt8;
typedef uint16_t	UInt16;
typedef int16_t		SInt16;

typedef uint32_t	UInt32;  
typedef int32_t		SInt32;
typedef uint64_t	UInt64;
typedef int64_t		SInt64;

#endif // __unix__

#if __has_feature(cxx_static_assert) || (defined(_MSC_VER) && _MSC_VER >= 1600)
static_assert(sizeof(UInt8)  == 1, "UInt8 expected to have size 1.");
static_assert(sizeof(SInt8)  == 1, "SInt8 expected to have size 1.");
static_assert(sizeof(UInt16) == 2, "UInt16 expected to have size 2.");
static_assert(sizeof(SInt16) == 2, "SInt16 expected to have size 2.");
static_assert(sizeof(UInt32) == 4, "UInt32 expected to have size 4.");
static_assert(sizeof(SInt32) == 4, "SInt32 expected to have size 4.");
static_assert(sizeof(UInt64) == 8, "UInt64 expected to have size 8.");
static_assert(sizeof(SInt64) == 8, "SInt64 expected to have size 8.");
#else
#warning no static_assert detected for this compiler.
#endif // __III_NO_VERIFY_BASIC_TYPES__


typedef SInt32 PacketSizeType;

typedef SInt32 SampleIndex;	// sample index (eg. well of multiwell plate)
typedef SInt32 CaptureIndex; // set of contiguously captured fields 
typedef SInt32 PositionIndex; // stage location index
typedef SInt32 TimepointIndex; // time point index
typedef UInt16 PlaneIndex; // Z plane index
typedef SInt32 ChannelIndex;
typedef SInt32 MaskIndex;

typedef SInt32 RatioChannelIndex;

typedef SInt32 FRETChannelIndex;

typedef UInt16 adu_type;
typedef UInt16 mask_type;

typedef UInt16 ColorChannelIndex;

#endif // __III_BASIC_TYPES__

//! III namespace. Contains III::SBReadFile, constants and III::Exception.
namespace III {

struct SBReadFile;

class Exception;

/** const UInt32 III::kAllExceptionsMasked
 *
 * \brief The exception propogation mask value such that no exception propogates
 * to the client
 *
 * \detail \copybrief III::kAllExceptionsMasked
 */
const UInt32 kAllExceptionsMasked = 0x00000000;

/** const UInt32 III::kNoExceptionsMasked
 *
 * \brief The exception propogation mask value such that all exceptions
 * propogate to the client
 *
 * \detail \copybrief III::kNoExceptionsMasked
 */
const UInt32 kNoExceptionsMasked = 0xFFFFFFFF;
}

extern "C"
{
    /** \file SBReadFile.h
     *
     * \fn III::SBReadFile* III_NewSBReadFile( const char* inFilename = NULL, III::UInt32 inExceptionMask = III::kAllExceptionsMasked )
     *
     * \brief Construct an III::SBReadFile instance in the free store. Returns
     * pointer to instance or NULL.
     *
     * If the provided path string is not NULL then the .sld file at the path is
     * opened.  If a failure occurs opening the file then the result depends on
     * the provided exception propogation mask.
     * 
     * If the exception mask is III::kAllExceptionsMasked (0x0) then then the
     * result after a failure occurs is as follows: Unless the failure a memory
     * allocation failure, return a valid (non-NULL) III::SBReadFile instance;
     * Otherwise return NULL.
     *
     * If a mask is provided then this may throw exceptions on failure (See
     * III::SBReadFile::Exceptions().) If the failure to allocate memory is masked
     * then this returns NULL.
     *
     * \param inFilename the path to the .sld file to be opened for read. The
     * default value is NULL. In this case no .sld file will be opened.
     * \param inExceptionMask the exception propogation mask. The default value
     * is III::kAllExceptionsMasked. In this case no exception is propogated to
     * the client.
     *
     * \sa III::SBReadFile::Exceptions
     * \sa III::SBReadFile::RdState
     * \sa III::SBReadFile::Open
     */
    III_EXPORT III::SBReadFile* III_NewSBReadFile( const char* inFilename = NULL
                                                 , UInt32 inExceptionMask = III::kAllExceptionsMasked );

    /** \file SBReadFile.h
     *
     * \fn void III_DeleteSBReadFile( III::SBReadFile* inSBReadFile )
     * \brief Delete an III::SBReadFile instance.
     *
     * \param inSBReadFile Pointer to III::SBReadFile instance. May be NULL
     * \subsection FailurePolicy Failure and Exception policy
     */
    III_EXPORT void III_DeleteSBReadFile( III::SBReadFile* inSBReadFile );

    /** \file SBReadFile.h
     *
     * \fn size_t III_GetErrorString( char* ioErrorString, 
     *                                const UInt32 inRdState, 
     *                                const UInt32 inErrorCode);
     * 
     * \param ioErrorString must point to an allocated buffer or NULL to only return string size
     *
     * \return 0 iff an internal failure building the error string occurs.
     * \return byte size of error string if ioErrorString is NULL.
     *         Includes NULL terminator.
     * \return byte size if ioErrorString is not NULL and now contains an error
     *         string of byte size.
     */
    III_EXPORT size_t III_GetErrorString(char* ioErrorString, 
                                         const UInt32 inRdState, 
                                         const UInt32 inErrorCode);

    //! Version information structure.
    struct III_Version
    {
        //! Release number. 
        UInt32 mRelease;

        /** If the SBReadFile interface is modified the major version is
         * incremented. With some changes a client compiled using a SBReadFile.h
         * with a major version less than the value returned by
         * III_SBReadFileVersion at runtime will still function as expected.
         * This is likely limited to changes where only additional functions are
         * added to the interface at the end of the interface definition.
         */
        UInt32 mMajor;

        /** If the changes do not include a change to the SBReadFile interface
         * the minor version is incremented. 
         */
        UInt32 mMinor;

        /** If the changes do not effect any functionality the build version is
         * incremented.
         */
        UInt32 mBuild;
    };

    //! The release of the SBReadFile library the client is compiled against.
    #define SBREADFILE_VERSION_RELEASE 0
    //! The major version of the SBReadFile library the client is compiled against.
    #define SBREADFILE_VERSION_MAJOR 6
    //! The minor version of the SBReadFile library the client is compiled against.
    #define SBREADFILE_VERSION_MINOR 0
    //! The build of the SBReadFile library the client is compiled against.
    #define SBREADFILE_VERSION_BUILD 0

    /** \file SBReadFile.h
     *
     * \fn void III_SBReadFileVersion( III_Version* outVersion );
     * 
     * \param Pointer to III_Version structure to fill in with the version
     * information.
     */
    III_EXPORT void III_SBReadFileVersion( III_Version* outVersion );
}

namespace III { 

/** Provides read access to SlideBook .sld files.
 *
 * Open a file for reading by providing the path either to 
 */
struct SBReadFile
{
    /** Error state bit flags and constants.
     *
     * If a failure occurs the error state of an instance is set and, depending
     * on policy, also causes an exception to be thrown. The error state is
     * either eGoodState or one or more error bit flags.
     *
     * On Windows the only supported configuration for exception propogation is
     * when the client links against the same CRT DLL version. 
     *
     * \sa Exceptions
     * \ingroup ErrorHandling
     */
	enum EErrorFlags {
      eEofbit  = 1 << 0, /** end-of-file while extracting from the file
                          * 
                          * Also sets eFailbit
                          */
      eFailbit = 1 << 1, /** A failure to extract a valid field from the file. 
                          *
                          * See GetLastError for additional details on the
                          * failure.
                          */
      eBadbit  = 1 << 2, //! a loss of integrity of the buffer
      eUnimplemented = 1 << 3, /** The requested functionality is not currently implemented.
                                *
                                * Depending on the requested functionality the
                                * reader can be placed into eGoodState by
                                * calling Clear()
                                */
      eUncategorized = 1 << 4, /** The failure does not fall into any previously mentioned categories.
                                *
                                * Indicates a bug internal to SBReadFile. Please
                                * contact support.
                                */
	  eGoodState = 0x00000000 //! where none of the previously mentioned bits are set
	};

	/** Closes the currently open file.
     *
     * If no file is currently open this has no effect. This includes when the
     * previous open failed.
     */
	virtual void Close() = 0;

	//! Return true if a file is open.
	virtual bool IsOpen() = 0;

	/** Opens the given file.
     *
     * If the open fails for any reason, the function calls SetState
     * accordingly. See EIOState and GetLastError.
     *
     * If a file is already open this fails.
     */
	virtual void Open(const char *inFilename) = 0;

	/** 
     * \defgroup ErrorHandling Error handling
     * @{
     */

	/** Get current error state. 
     *
     * If the eFailbit is set GetLastError can provide additional information on
     * the failure.
     */
	virtual UInt32 RdState() const throw () = 0;

    /** Return true if there is no error state.
     */
    virtual bool Good() const throw () = 0;

    /** If possible clear the error state.
     *
     * If this method returns false then SBReadFile cannot recover from the
     * previously encountered failure. 
     */
    virtual bool Clear() = 0;

    /** Provides an extended failure code in the case eFailbit is set. 
     *
     * This failure code can be provided to GetErrorString for a description of
     * the failure.
     */
	virtual UInt32 GetLastError() const throw () = 0;

    /** Returns the current exception policy mask.
     * 
     * If the exception policy mask is equal to kAllExceptionsMasked then no
     * failure propogates an exception to the client. 
     *
     * For each bit in EErrorFlags iff that bit is set in the returned word will
     * exceptions of that type of failure be propogated to the client.
     *
     * On Windows the only supported configuration for exception propogation is
     * when the client links against the same CRT DLL version. 
     *
     * \sa III::Exception
     */
    virtual UInt32 Exceptions () const throw () = 0;

    /** Sets the current exception policy mask.
     *
     * If the exception policy mask is equal to kAllExceptionsMasked then no
     * failure propogates an exception to the client. 
     *
     * For each bit in EErrorFlags iff that bit is set in the provided word will
     * exceptions of that type of failure be propogated to the client.
     *
     * On Windows the only supported configuration for exception propogation is
     * when the client links against the same CRT DLL version. 
     *
     */
    virtual void Exceptions( UInt32 inMask) = 0;

    /** @} */

    /** 
     * \defgroup RangeData Range data.
     * @{
     */

	/** C: Number of contiguously captured fields
     *
     * Captures are 0-based indexed. 
     */
	virtual CaptureIndex GetNumCaptures() const = 0;

	//! P: number of stage locations per capture
	virtual PositionIndex GetNumPositions(const CaptureIndex inCaptureIndex) const = 0;

	//! X: number of pixel columns per field
	virtual SInt32 GetNumXColumns(const CaptureIndex inCaptureIndex) const = 0;
	
	//! Y: number of pixel rows per field
	virtual SInt32 GetNumYRows(const CaptureIndex inCaptureIndex) const = 0;
	
	//! Z: number of planes per field
	virtual SInt32 GetNumZPlanes(const CaptureIndex inCaptureIndex) const = 0;
	
	//! T: number of timepoints per field
	virtual TimepointIndex GetNumTimepoints(const CaptureIndex inCaptureIndex) const = 0;
	
	//! L: optical configuration index (llambda)
	virtual ChannelIndex GetNumChannels(const CaptureIndex inCaptureIndex) const = 0;

    /** @} */

    /**
     * \defgroup MetaData Meta Data
     * @{
     */

	/** Get exposure time, in milliseconds, for a particular channel
     * 
     * If inCaptureIndex or inChannelIndex are not valid indices then the reader
     * will be placed in the failure state. If the exception policy masked the
     * failure exception then the return value will be 0.
     *
     * \return Time of exposure.
     *
     * \sa GetNumCaptures 
     * \sa GetNumChannels
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual UInt32 GetExposureTime(const CaptureIndex inCaptureIndex, const ChannelIndex inChannelIndex) const = 0;

	/** get micron size per pixel
     *
     * The return value indicates if a valid dimension's value is 0 because
     * there is no micron size associated with that dimension.
     *
     * For a 2D capture only the X and Y are considered valid dimensions. 
     *
     * If inCaptureIndex is not a valid capture index (See GetNumCaptures) then
     * the reader will be placed in the failure state. If the exception policy
     * masks the failure exception then the values of outX, outY and outZ will
     * be undefined and false will be returned. 
     *
     * \return true if the units of the dimensions are in microns.
     *
     * \sa GetNumCaptures 
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual bool GetVoxelSize(const CaptureIndex inCaptureIndex, float& outX, float& outY, float& outZ) const = 0;

	/** get X position in microns of center of image
     *
     * If inCaptureIndex or inPositionIndex are not valid indices then the
     * reader will be placed in the failure state. If the exception policy masks
     * the failure exception then the return value will be 0.0.
     *
     * \sa GetNumCaptures 
     * \sa GetNumPositions
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual float GetXPosition(const CaptureIndex inCaptureIndex, const PositionIndex inPositionIndex) const = 0;

	/** get Y position in microns of center of image
     *
     * If inCaptureIndex or inPositionIndex are not valid indices then the
     * reader will be placed in the failure state. If the exception policy masks
     * the failure exception then the return value will be 0.0.
     *
     * \sa GetNumCaptures 
     * \sa GetNumPositions
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual float GetYPosition(const CaptureIndex inCaptureIndex, const PositionIndex inPositionIndex) const = 0;

	/** get Z plane position in microns of image plane
     *
     * If inCaptureIndex, inPositionIndex, or inZPlaneIndex are not valid
     * indices then the reader will be placed in the failure state. If the
     * exception policy masks the failure exception then the return value will
     * be 0.0.
     *
     * \sa GetNumCaptures 
     * \sa GetNumPositions
     * \sa GetNumZPlanes
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual float GetZPosition( const CaptureIndex inCaptureIndex, 
                                const PositionIndex inPositionIndex, 
                                const PlaneIndex inZPlaneIndex = 0) const = 0;

	/** get montage row
     *
     * If inCaptureIndex or inPositionIndex are not valid indices then the
     * reader will be placed in the failure state. If the exception policy masks
     * the failure exception then the return value will be 0.
     *
     * \sa GetNumCaptures 
     * \sa GetNumPositions
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual UInt32 GetMontageRow(const CaptureIndex inCaptureIndex, const PositionIndex inPositionIndex) const = 0;

	/** get montage column
     *
     * If inCaptureIndex or inPositionIndex are not valid indices then the
     * reader will be placed in the failure state. If the exception policy masks
     * the failure exception then the return value will be 0.
     *
     * \sa GetNumCaptures 
     * \sa GetNumPositions
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual UInt32 GetMontageColumn(const CaptureIndex inCaptureIndex, const PositionIndex inPositionIndex) const = 0;

	/** get elapsed time at a particular timepoint in milliseconds
     *
     * If inCaptureIndex or inTimepointIndex are not valid indices then the
     * reader will be placed in the failure state. If the exception policy masks
     * the failure exception then the return value will be 0.
     *
     * \sa GetNumCaptures 
     * \sa GetNumTimepoints
     * \sa Exceptions 
     * \sa III::Exception 
     */
	virtual UInt32 GetElapsedTime(const CaptureIndex inCaptureIndex, const TimepointIndex inTimepointIndex) const = 0;

	/** get channel name, ioChannelName  must point to an allocated buffer or NULL to only return string size
     *
     * If inCaptureIndex or inChannelIndex are not valid indices then the
     * reader will be placed in the failure state. If the exception policy masks
     * the failure exception then the return value will be 0.
     *
     * \param ioChannelName must point to an allocated buffer or NULL to only
     *                      return string size.
     *
     * \return 0 iff there is a failure determining the channel name and
     *         exceptions are disabled.
     * \return byte size of channel name string if ioChannelName is NULL.
     *         Includes NULL terminator.
     * \return byte size if ioChannelName is not NULL and now contains a channel
     *         name string of byte size.
     */
	virtual UInt32 GetChannelName(char *ioChannelName, const CaptureIndex inCaptureIndex, const ChannelIndex inChannelIndex) const = 0;

	/** get lens name, ioLensName must point to an allocated buffer or NULL to only return string size
     *
     * If inCaptureIndex is not a valid index then the reader will be placed in
     * the failure state. If the exception policy masks the failure exception
     * then the return value will be 0.
     *
     * \param ioLensName must point to an allocated buffer or NULL to only return string size.
     *
     * \return 0 iff there is a failure determining the lens name and
     *         exceptions are disabled.
     * \return byte size of lens name string if ioLensName is NULL.
     * \return byte size if ioLensName is not NULL and now contains a lens name
     *         string of byte size.
     */
	virtual UInt32 GetLensName(char *ioLensName, const CaptureIndex inCaptureIndex) const = 0;

	/** get effective magnification used for a particular capture (lens and magnification changer combined)
     *
     * If inCaptureIndex is not a valid index then the reader will be placed in
     * the failure state. If the exception policy masks the failure exception
     * then the return value will be 0.0.
     *
     * \return magnification
     */
	virtual float GetMagnification(const CaptureIndex inCaptureIndex) const = 0;

	/** get image name, ioImageName must point to an allocated buffer or NULL to only return string size
     *
     * If inCaptureIndex is not a valid index then the reader will be placed in
     * the failure state. If the exception policy masks the failure exception
     * then the return value will be 0.
     *
     * \param ioImageName must point to an allocated buffer or NULL to only return string size.
     *
     * \return 0 iff there is an error determining image name string and
     *         the exception is masked.
     * \return byte size of image name string if ioImageName is NULL.
     * \return byte size if ioImageName is not NULL and now contains an image name string of
     *         byte size.
     */
	virtual UInt32 GetImageName(char *ioImageName, const CaptureIndex inCaptureIndex) const = 0;

	/** get image comments, ioImageComments must point to an allocated buffer or NULL to only return string size
     *
     * If inCaptureIndex is not a valid index then the reader will be placed in
     * the failure state. If the exception policy masks the failure exception
     * then the return value will be 0.
     *
     * \param ioImageComments must point to an allocated buffer or NULL to only return string size.
     *
     * \return byte size of image comments string if ioImageComments is NULL.
     * \return 1 if ioImageComments is not NULL and now contains the image
     * comments.
     */
	virtual UInt32 GetImageComments(char *ioImageComments, const CaptureIndex inCaptureIndex) const = 0;

	/** get image capture date, ioCaptureDate must point to an allocated buffer or NULL to only return string size
     *
     * If inCaptureIndex is not a valid index then the reader will be placed in
     * the failure state. If the exception policy masks the failure exception
     * then the return value will be 0.
     *
     * \param ioCaptureDate must point to an allocated buffer or NULL to only return string size.
     *
     * \return byte size of image comments string if ioCaptureDate is NULL.
     * \return 1 if ioCaptureDate is not NULL and now contains the image
     * comments.
     */
	virtual UInt32 GetCaptureDate(char *ioCaptureDate, const CaptureIndex inCaptureIndex) const = 0;


    /** @} 
     */

	/**
     * \defgroup ReadData Read Image Data
     * @{
     */
	
    /** Read image data into outPlaneBuf with specified byte stride.
     *
	 * fill memory buffer 'outPlaneBuf' with a single 2d plane of image data
	 * outPlaneBuf must have allocated size > rows x stride
     *
     * Stride must be greater than columns x 2.
     *
     * \returns true if all of the requested data was read into outPlaneBuf.
     * \returns false if any of the requested data could not be read into
     * outPlaneBuf. Will not return false if exception policy does not mask the
     * eFailbit exception.
     */
	virtual bool ReadImagePlaneBuf(UInt16* outPlaneBuf, std::size_t inByteStride,
                                   const CaptureIndex inCaptureIndex,
                                   const PositionIndex inPositionIndex,
                                   const TimepointIndex inTimepointIndex,
                                   const PlaneIndex inZPlaneIndex,
                                   const ChannelIndex inChannelIndex) const = 0;

    /** Read image data into outPlaneBuf with columns x 2 stride.
     *
	 * fill memory buffer 'outPlaneBuf' with a single 2d plane of image data
	 * outPlaneBuf must have allocated size > rows x columns x 2
     *
     * \returns true if all of the requested data was read into outPlaneBuf.
     * \returns false if any of the requested data could not be read into
     * outPlaneBuf. Will not return false if exception policy does not mask the
     * eFailbit exception.
     */
	virtual bool ReadImagePlaneBuf(UInt16* outPlaneBuf,
                                   const CaptureIndex inCaptureIndex,
                                   const PositionIndex inPositionIndex,
                                   const TimepointIndex inTimepointIndex,
                                   const PlaneIndex inZPlaneIndex,
                                   const ChannelIndex inChannelIndex) const = 0;
    /** @} 
     */

	/**
     * \defgroup AuxData Read Auxiliary Image Data
     * @{
     */

   /** Read auxiliary data channel XML descriptor into buffer 
     *
	 * fill memory buffer 'outXMLDescriptorBuf' with XML description of specified auxiliary data channel
	 * 
     *
     * \returns true if all of the requested data was read into outXMLDescriptorBuf.
     * \returns false if any of the requested data could not be read.
     */
	virtual UInt32 GetAuxDataXMLDescriptor(const CaptureIndex inCaptureIndex, const size_t inAuxDataChannel, 
		char* outXMLDescriptorBuf) const = 0;

    /** Return number of elements for specified auxiliary data channel
     *
	 * \returns number of data elements in specified auxiliary data channel and sets outType to one of
	 * EValueType if outType is not NULL.
     * \returns 0 if any of the requested data does not exist.
     */
	virtual std::size_t GetAuxDataNumElements(const CaptureIndex inCaptureIndex, const size_t inAuxDataChannel, int* outType) const = 0;

   /** Read float auxiliary data channel data into buffer
     *
	 * fill memory buffer 'outAuxDataBuf' with float data from specified auxiliary data channel
	 * outAuxDataBuf must have allocated size > inBufSize * sizeof(float)
     *
     * \returns true if all of the requested data was read into outAuxDataBuf.
     * \returns false if any of the requested data could not be read into
     * outAuxDataBuf. Will not return false if exception policy does not mask the
     * eFailbit exception.
     */
	virtual bool GetAuxFloatData(const CaptureIndex inCaptureIndex, const size_t inAuxDataChannel, 
		float* outAuxDataBuf, std::size_t inNumElements) const = 0;

   /** Read double auxiliary data channel data into buffer
     *
	 * fill memory buffer 'outAuxDataBuf' with double data from specified auxiliary data channel
	 * outAuxDataBuf must have allocated size > inBufSize * sizeof(double)
     *
     * \returns true if all of the requested data was read into outAuxDataBuf.
     * \returns false if any of the requested data could not be read into
     * outAuxDataBuf. Will not return false if exception policy does not mask the
     * eFailbit exception.
     */
	virtual bool GetAuxDoubleData(const CaptureIndex inCaptureIndex, const size_t inAuxDataChannel, 
		double* outAuxDataBuf, std::size_t inNumElements) const = 0;

   /** Read SInt32 auxiliary data channel data into buffer
     *
	 * fill memory buffer 'outAuxDataBuf' with SInt32 data from specified auxiliary data channel
	 * outAuxDataBuf must have allocated size > inBufSize * sizeof(SInt32)
     *
     * \returns true if all of the requested data was read into outAuxDataBuf.
     * \returns false if any of the requested data could not be read into
     * outAuxDataBuf. Will not return false if exception policy does not mask the
     * eFailbit exception.
     */
	virtual bool GetAuxSInt32Data(const CaptureIndex inCaptureIndex, const size_t inAuxDataChannel, 
		SInt32* outAuxDataBuf, std::size_t inNumElements) const = 0;

   /** Read serialized auxiliary data channel data into buffer
     *
	 * fill memory buffer 'outAuxDataBuf' with char data from specified element of 
	 * specified auxiliary data channel outAuxDataBuf must have allocated size > inBufSize * sizeof(char)
     *
     * \returns size of the requested data if outAuxDataBuf is NULL.
     * \returns false if any of the requested data could not be read into
     * outAuxDataBuf. Will not return false if exception policy does not mask the
     * eFailbit exception.
     */
	virtual int GetAuxSerializedData(const CaptureIndex inCaptureIndex, const size_t inAuxDataChannel, 
		const size_t inElementIndex, char* outAuxDataBuf, std::size_t inNumElements) const = 0;

    /** @} 
     */

protected:
    virtual ~SBReadFile() = 0;
};

/** Base of SBReadFile exception hierarchy.
 *
 * \note This does not inherit from the std exception hierarchy on purpose. If
 * this is to prevent confusion between the client system and the SBReadFile
 * library over the std::exception definition.
 */
class Exception
{
public:
    virtual ~Exception() = 0;

    /** RdState of the SBReadFile when this exception thrown.
     */
    virtual UInt32 RdState() const = 0;

    /** GetLastError of the SBReadFile when this exception thrown.
     */
    virtual UInt32 GetLastError() const = 0;

    /** Long description of the exception.
     *
     * The string is deallocated when this Exception instance is destroyed.
     */
    virtual const char* GetDescription() const = 0;
};

namespace ErrorCodes {

enum ECode
{
    eNone = 0,
    eUncategorizedFailure = 1,
    eUnableToOpen,
    eInvalidSlideDocument,
    eInvalidCaptureIndex,
    eMaxCode
};

/* namespace ErrorCodes */ };

/** Simple entirely inlined defined scoped pointer for SBReadFile.
 * Since SBReadFile must be heap allocated by the DLL all allocation &
 * deallocation must be done via III_NewSBReadFile & III_DeleteSBReadFile.
 *
 * To make this easier for clients this scoped pointer is provided which assures
 * III_DeleteSBReadFile is called.
 */
class SBReadFilePtr
{
public:
    SBReadFilePtr(const char* inFilePath = NULL, 
                  UInt32 inExceptionMask = kAllExceptionsMasked) :
        mPtr(III_NewSBReadFile(inFilePath, inExceptionMask))
    { }

    SBReadFilePtr(SBReadFile* inPtr) :
        mPtr(inPtr)
    { }

    ~SBReadFilePtr()
    { III_DeleteSBReadFile(mPtr); }

    const SBReadFile* operator->() const
    { return mPtr; }

    SBReadFile* operator->() 
    { return mPtr; }

    SBReadFile* Release()
    {
        SBReadFile* outPtr = mPtr;
        mPtr = NULL;
        return outPtr;
    }

    const SBReadFile* Get() const
    { return mPtr; }

    SBReadFile* Get() 
    { return mPtr; }

private:
    SBReadFile* mPtr;
};

/* namespace III */ };

#ifdef __unix__
#pragma GCC visibility pop
#endif
