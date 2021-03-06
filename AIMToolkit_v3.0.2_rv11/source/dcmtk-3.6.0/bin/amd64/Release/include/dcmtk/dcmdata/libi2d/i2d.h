/*
 *
 *  Copyright (C) 2001-2010, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *
 *  Module:  dcmdata
 *
 *  Author:  Michael Onken
 *
 *  Purpose: Class to control conversion of image format to DICOM
 *
 *  Last Update:      $Author: joergr $
 *  Update Date:      $Date: 2010-10-14 13:15:46 $
 *  CVS/RCS Revision: $Revision: 1.13 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#ifndef I2D_H
#define I2D_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/libi2d/i2doutpl.h"
#include "dcmtk/dcmdata/libi2d/i2dplvlp.h"
#include "dcmtk/dcmdata/libi2d/i2dplsc.h"
#include "dcmtk/dcmdata/libi2d/i2djpgs.h"
#include "dcmtk/dcmdata/dcpixel.h"

class Image2Dcm
{

public:

  /** Constructor, initializes command line options
   *  @return none
   */
  Image2Dcm();

  /** Start the conversion. Needs a fully configured input plugin
   *  and a fully configured output plugin to operate. Returns
   *  a dataset with the resulting DICOM object.
   *  @param inputPlug - [in] The input plugin to read pixel data
   *  @param outPlug - [in] The output plugin for specific SOP class output
   *  @param resultDset - [out] The DICOM object resulting from the conversion
   *  @param proposedTS - [out] The proposed transfer syntax (needed e. g.
   *                            by JPEG input plugin)
   *  @return EC_Normal, if successfull, error otherwise
   */
  OFCondition convert(I2DImgSource *inputPlug,
                      I2DOutputPlug *outPlug,
                      DcmDataset*& resultDset,
                      E_TransferSyntax& proposedTS);

  /** Sets a DICOM file that should serve as a template for the resulting
    * DICOM object. Only the dataset of the given file is imported.
    * @param file - [in] The filename of the template DICOM file
    * @return none
    */
  void setTemplateFile(const OFString& file);

  /** Set file from which patient/study/series data should be imported from.
   *  @param file - [in] The DICOM file to read from
   *  @return none
   */
  void setSeriesFrom(const OFString& file);

  /** Set file from which patient/study/series data should be imported from.
   *  @param file - [in] The DICOM file to read from
   *  @return none
   */
  void setStudyFrom(const OFString& file);

  /** Enables/Disables incrementing instance number taken over from DICOM file.
   *  This can either be a the study/series file or the template file
   *  provided.
   *  @param incInstNo - [in] If true, the instance number will be incremented
   *  @return none
   */
  void setIncrementInstanceNumber(OFBool incInstNo);

  /** Enables/disables autotmatic insertion of the value "ISO_IR100" as
    * a value for the Specific Character Set attribute. If disabled,
    * no value is inserted for Specifific Character Set but instead
    * must be provided by other means (see setTemplateFile(), setSeriesFrom(),
    * setStudyFrom(), setOverrideKeys()). The insertion is applied after
    * loading the (optional) template file; the value might be overwritten
    * by the value copied by setStudy/Series/OverrideKeys.
    * @param insertLatin1 - [in] Enable/disable insertion of Iso Latin 1
    * @return none;
    */
  void setISOLatin1(OFBool insertLatin1);

  /** Specifies some attributes that should be inserted after conversion.
   *  They will override any identical attributes already existing in the
   *  converted result DICOM object. The override keys are applied at the very
   *  end of the conversion and do not undergoe any validity checking.
   *  @param ovkeys [in] override keys that can be tags, dictionary names
   *                     and paths (see DcmPath for syntax). Also it is
   *                     permitted to set a value if appropriate, e. g.
   *                     "PatientName=Doe^John" would be a valid override
   *                     key.
   *  @return none
   */
  void setOverrideKeys(const OFList<OFString>& ovkeys);

  /** Enable/Disable basic validity checks for output dataset
   *  @param doChecks - [in] OFTrue enables checking, OFFalse turns it off.
   *  @param insertMissingType2 - [in] If true (default), missing type 2
   *         attributes are inserted automatically
   *  @param inventMissingType1 - [in] If true , missing type 1
   *         attributes are inserted automatically with a predefined
   *         value (if possible). An existing empty type 1 attribute is
   *         assigned a value, too.
   *  @return none
   */
  void setValidityChecking(OFBool doChecks,
                           OFBool insertMissingType2 = OFTrue,
                           OFBool inventMissingType1 = OFTrue);

  /** Destructor, frees plugin memory
   *  @return none
   */
  ~Image2Dcm();

protected:

  /** Cleans up template for future insertion of pixel data information, ie
    * generally removes attributes from image pixel module
    * @param targetDset - [out] The dataset that should be cleand up
    * @return none
    */
  void cleanupTemplate(DcmDataset *targetDset);

  /** Parses patient/study/series level attributes from file as defined
   *  on command line and fills internal attributes accordingly.
   *  @param targetDset - [out] The dataset to write the attributes to
   *  @return EC_Normal, if successful, error otherwise
   */
  OFCondition applyStudyOrSeriesFromFile(DcmDataset *targetDset);

  /** Increments instance number in given dataset
   *  @param targetDset - [in/out] Dataset to increment instance number in
   *  @return EC_Normal, if successful, error otherwise
   */
  OFCondition incrementInstanceNumber(DcmDataset *targetDset);

  /** Generates and inserts missing UIDs, ie UIDs not taken over
   *  from --dataset-from --study from and --series-from options
   *  @param dset - [in] The dataset to write the UIDs to
   *  @return EC_Normal, if successful, error otherwise
   */
  OFCondition generateUIDs(DcmDataset *dset);

  /** Reads pixel data and corresponding attributes like rows etc. from image
   *  file and inserts them into dataset.
   *  @param imageSource - [in] The input plugin that actually reads the pixel data
   *  @param dset - [out] The dataset to export the pixel data attributes to
   *  @param outputTS - [out] The proposed transfex syntax of the dataset
   *  @return EC_Normal, if successful, error otherwise
   */
  OFCondition readAndInsertPixelData( I2DImgSource* imageSource,
                                      DcmDataset* dset,
                                      E_TransferSyntax& outputTS);

  /** Do some completeness / validity checks. Should be called when
   *  dataset is completed and is about to be saved.
   *  @param dataset - [in] The dataset to check
   *  @return Error string if error occurs, empty string otherwise
   */
  OFString isValid(DcmDataset& dataset) const;

  /** Copy override keys over existing keys in given dataset.
   *  @param outputDset - [out] dataset to which the override keys are copied
   *  @return Error code if error occurs, EC_Normal otherwise
   */
   OFCondition applyOverrideKeys(DcmDataset *outputDset);

   /** Inserts "ISO_IR100" in the attribute "Specific Character Set".
    *  Overwrites any existing value.
    *  @param outputDset - [out] The dataset to write to
    *  @return EC_Normal if insertion is successfull, error code otherwise
    */
   OFCondition insertLatin1(DcmDataset *outputDset);

   /** Check whether given type 1 attribute is present and has a length > 0.
    *  @param  key - [in] The attribute tag check
    *  @param  targetDset - [out] targetDset
    *  @param  defaultValue - [in] value to be inserted if attribute is missing
    *                         (needs invent option for type 1 attributes enabled)
    *  @return A string with an error message if attribute is not present
    *          or has length of 0
    */
   OFString checkAndInventType1Attrib(const DcmTagKey& key,
                                      DcmDataset* targetDset,
                                      const OFString& defaultValue = "") const;


   /** Inserts an empty element with the given tag into a dataset
    *  @param  key - [in] The attribute tag to check
    *  @param  targetDset - [out] targetDset
    *  @return A string with an error message if attribute is not present
    *          nor gets inserted automatically (see m_inventMissingType2Attribs)
    */
   OFString checkAndInventType2Attrib(const DcmTagKey& key,
                                      DcmDataset* targetDset) const;

private:

  /** Correctly inserts encapsulated pixel data.
   *  @param dset [in] - The dataset to which we should add this.
   *  @param pixData [in] - The data to add.
   *  @param length [in] - The length of pixData.
   *  @param outputTS [in] - The output transfer syntax to be used
   *  @return EC_Normal, if successfull, error otherwise.
   */
  OFCondition insertEncapsulatedPixelData(DcmDataset* dset,
                                          char *pixData,
                                          Uint32 length,
                                          const E_TransferSyntax& outputTS) const;

  /* Attributes for writing DICOM dataset */

  /// These attributes are applied to the dataset after conversion
  /// (and are not checked by the isValid() function)
  OFList<OFString> m_overrideKeys;

  /// If not empty, the DICOM file specified in this variable is used
  /// as a base for the DICOM image file to be created, ie. all attributes
  /// are taken over from this template file
  OFString m_templateFile;

  /// If true, patient and study data is read from file
  OFBool m_readStudyLevel;

  /// If true, patient, study and series data is read from file
  OFBool m_readSeriesLevel;

  /// File to read study and series from
  OFString m_studySeriesFile;

  /// If true, Instance Number ist read from file and incremented by 1
  OFBool m_incInstNoFromFile;

  ///  If true, some simple attribute checks (missing type 2 attributes or
  ///  missing type 1 values) are omitted
  OFBool m_disableAttribChecks;

  /// If true, missing type 2 elements are inserted automatically (empty value).
  /// Enabled per default.
  OFBool m_inventMissingType2Attribs;

  /// If true, missing type 1 elements are inserted automatically with a
  /// predefined value, if possible. Disabled per default.
  OFBool m_inventMissingType1Attribs;

  /// If enabled, ISO_IR100 (ISO Latin 1) is inserted as a standard value
  /// for attribute "Specific Character Set". Insertion takes place after
  /// loading (optional) template file.
  OFBool m_insertLatin1;
};

#endif // I2D_H

/*
 * CVS/RCS Log:
 * $Log: i2d.h,v $
 * Revision 1.13  2010-10-14 13:15:46  joergr
 * Updated copyright header. Added reference to COPYRIGHT file.
 *
 * Revision 1.12  2010-08-09 13:03:07  joergr
 * Updated data dictionary to 2009 edition of the DICOM standard. From now on,
 * the official "keyword" is used for the attribute name which results in a
 * number of minor changes (e.g. "PatientsName" is now called "PatientName").
 *
 * Revision 1.11  2010-03-25 09:44:07  onken
 * Pixel data is now already marked with the correct transfer syntax in memory
 * not only when writing to disk. This permits conversion in memory, e. g. for
 * sending the converted DICOM images directly over the network.
 *
 * Revision 1.10  2009-11-04 09:58:08  uli
 * Switched to logging mechanism provided by the "new" oflog module
 *
 * Revision 1.9  2009-09-30 08:05:25  uli
 * Stop including dctk.h in libi2d's header files.
 *
 * Revision 1.8  2009-07-16 14:23:37  onken
 * Extended Image2Dcm engine to also work for uncompressed pixel data input.
 *
 * Revision 1.7  2009-07-10 13:16:16  onken
 * Added path functionality for --key option and lets the code make use
 * of the DcmPath classes.
 *
 * Revision 1.6  2009-03-31 13:06:09  onken
 * Changed implementation of lossy compression attribute detection and writing.
 *
 * Revision 1.4  2009-01-16 09:51:55  onken
 * Completed doxygen documentation for libi2d.
 *
 * Revision 1.3  2008-01-16 16:32:23  onken
 * Fixed some empty or doubled log messages in libi2d files.
 *
 * Revision 1.2  2008-01-16 15:07:40  onken
 * Moved library "i2dlib" from /dcmdata/libsrc/i2dlib to /dcmdata/libi2d
 *
 * Revision 1.2  2008-01-11 14:17:53  onken
 * Added various options to i2dlib. Changed logging to use a configurable
 * logstream. Added output plugin for the new Multiframe Secondary Capture SOP
 * Classes. Added mode for JPEG plugin to copy exsiting APPn markers (except
 * JFIF). Changed img2dcm default behaviour to invent type1/type2 attributes (no
 * need for templates any more). Added some bug fixes.
 *
 * Revision 1.1  2007/11/08 15:58:55  onken
 * Initial checkin of img2dcm application and corresponding library i2dlib.
 *
 *
 */
