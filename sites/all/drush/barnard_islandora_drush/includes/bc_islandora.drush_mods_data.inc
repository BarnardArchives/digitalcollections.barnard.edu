<?php

/**
 * @file
 * Helper functions for generating MODS metadata during bulk ingest.
 */

/**
 * Generates MODS records: RG BC13-58 - Scholar and Feminist Audio.
 *
 * @author Ben Rosner, @br2490
 *
 * CSV Row processor for Barnard College S&F Audio.
 * The formatting of this section is purposeful.
 * Pay attention to spacing in the creation of variables.
 *
 * We use single quote literals for static content, and heredoc strings for
 * dynamic content. We also use heredoc to gen the final MODs string.
 */
function _bc_islandora_rg1358_audio_mods($mods_data, $type) {
  $identifier_xml = <<<PID
  <identifier type="local">${mods_data['id']}</identifier>
PID;

  $titleinfo_title_xml = <<<TITLE
  <titleInfo>
    <title>${mods_data['title']}</title>
  </titleInfo>
TITLE;

  $related_item_xml = '
  <relatedItem type="host" displayLabel="Record Group">
    <titleInfo>
      <title>Academic Departments and Programs</title>
    </titleInfo>
  </relatedItem>
  <relatedItem type="host">
    <titleInfo>
      <title>Barnard Center for Research on Women</title>
    </titleInfo>
    <identifier type="local">BC13-58</identifier>
    <typeOfResource>mixed material</typeOfResource>
  </relatedItem>';

  // Creators - mods:name.
  $creators = $mods_data['creators'];
  $creator_xml = '';
  foreach ($creators as $creator) {
    if (!empty($creator['name'])) {
      if (!empty($creator['uri'])) {
        $creator_xml .= <<<CR_URI
  <name type="personal" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="${creator['uri']}">
CR_URI;
      }
      else {
        $creator_xml .= '
  <name type="personal" authority="local">';
      }
      $creator_xml .= <<<CXML

    <namePart>${creator['name']}</namePart>
    <role>
      <roleTerm type="text" authority="marcrelator">Creator</roleTerm>
    </role>
  </name>
CXML;
    }
  }

  // This variable is pretty sneaky, it contains more than the date. We
  // initialize the variable here with the start of mods:originInfo.
  $date_xml = '
  <originInfo>';

  // This generates MODS records for an audio object or compound
  if ($type !== 'page') {

    // Collections do not require a physical description.
    if ($type !== 'collection') {
      $extent_xml = <<<PDESCRIP
  <physicalDescription>
    <form authority="marcform">electronic</form>
    <extent>${mods_data['extent']}</extent>
    <digitalOrigin>reformatted digital</digitalOrigin>
    <internetMediaType>audio/mpeg</internetMediaType>
  </physicalDescription>
PDESCRIP;
    }

    $date_xml .= '
    <place>
      <placeTerm type="text" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79007751">New York (N.Y.)</placeTerm>
    </place>
    <place>
      <placeTerm type="code" authority="marccountry">nyu</placeTerm>
    </place>
    <publisher>Online collection published by Barnard Archives and Special Collections, Barnard College.</publisher>';

    $description_xml = <<<ABSTRACT
  <abstract>${mods_data['description']}</abstract>
ABSTRACT;

    $shelf_location = !empty($mods_data['location']) ? "
    <shelfLocator>${mods_data['location']}</shelfLocator>" : '';

    $location_xml = <<<LOCATION_LOCATION_LOCATION
  <location displayLabel="not a PURL">
    <physicalLocation authority="marcorg">NNC</physicalLocation>
    <physicalLocation authority="marcorg">NyNyBAW</physicalLocation>
    <physicalLocation>Barnard College Library (New York, N.Y.)</physicalLocation>{$shelf_location}
  </location>
LOCATION_LOCATION_LOCATION;

    $language_xml = '
  <language>
    <languageTerm type="text">English</languageTerm>
    <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
  </language>';

//    $access_condition_xml = '
//  <accessCondition type="useAndReproduction">Contact the Archives at archives@barnard.edu for more information on access and use.</accessCondition>';

    // We need to decode this since it's being passed in as XML already. I know.
    $mods_data['access_condition'] = htmlspecialchars_decode($mods_data['access_condition']);
    $access_condition_xml = <<<ACL
  {$mods_data['access_condition']}
ACL;

    $record_info_xml = '
  <recordInfo>
    <recordContentSource>Barnard Archives and Special Collections</recordContentSource>
    <recordContentSource authority="marcorg">NNC</recordContentSource>
    <recordContentSource authority="marcorg">NyNyBAW</recordContentSource>
    <languageOfCataloging>
      <languageTerm type="text" authority="iso639-2b">English</languageTerm>
      <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
    </languageOfCataloging>
  </recordInfo>';
  }

  // Dates - mods:originInfo/date.
  // Exact Date range.
  $date_xml .= <<<DATESTR

    <dateCreated encoding="w3cdtf" keyDate="yes">${mods_data['date_exact']}</dateCreated>
  </originInfo>
DATESTR;

  // Everything but collections need to have a related collection and PID.
  if ($type !== "collection") {
    $resource_type_xml = <<<TYPE
  <typeOfResource>sound recording</typeOfResource>
TYPE;

    $genre_xml = <<< GNR
  <genre authority="aat" authorityURI="http://vocab.getty.edu/aat" valueURI="http://vocab.getty.edu/page/aat/300028661">audiocassettes</genre>
GNR;

    // Topics - mods:subject.
    // Static header (single quote, no eval)
    $subject_xml = '
  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
      <namePart>Barnard College</namePart>
    </name>
  </subject>
  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/no90023871">
      <namePart>Barnard Center for Research on Women</namePart>
    </name>
  </subject>
  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n98110944">
      <namePart>Scholar and the Feminist Conference</namePart>
    </name>
  </subject>';
    // Subject terms (requires both a topic and a topic uri).
    $subjects = $mods_data['topics'];
    foreach ($subjects as $topic) {
      if (!empty($topic['topic']) && !empty($topic['uri'])) {
        $subject_xml .= <<<SUBJ

  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="${topic['uri']}">
    <topic>${topic['topic']}</topic>
  </subject>
SUBJ;
      }
    }
  }

  // Build the final return mods record. Better hope you intended everything.
  $mods_xml = <<<MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">

{$identifier_xml}
{$titleinfo_title_xml}
{$creator_xml}
{$resource_type_xml}
{$genre_xml}
{$date_xml}
{$extent_xml}
{$description_xml}
{$location_xml}
{$language_xml}
{$subject_xml}
{$related_item_xml}
{$record_info_xml}
{$access_condition_xml}

</mods>
MODS;

  // Normalize (correct xml formatting if necessary).
  $ret = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $mods_xml);

  return $ret;
}

/**
 * Generates Scrapbook MODS records: RG: BC15.
 *
 * @author Ben Rosner, all blame to: brosner@barnard.edu
 *
 * CSV Row processor for Barnard College Alumnae Scrapbooks.
 * The formatting of this section is purposeful.
 * Pay attention to spacing in the creation of variables.
 *
 * We use single quote literals for static content, and heredoc strings for
 * dynamic content. We also use heredoc to gen the final MODs string.
 */
function _bc_islandora_rg15_mods($mods_data, $type) {
  $identifier_xml = <<<PID
  <identifier type="local">${mods_data['id']}</identifier>
PID;

  $titleinfo_title_xml = <<<TITLE
  <titleInfo>
    <title>${mods_data['title']}</title>
  </titleInfo>
TITLE;

  $related_item_xml = '
  <relatedItem type="host" displayLabel="Record Group">
    <titleInfo>
      <title>Alum Scrapbooks</title>
    </titleInfo>
  </relatedItem>';

  // Creators - mods:name.
  $creators = $mods_data['creators'];
  $creator_xml = '';
  foreach ($creators as $creator) {
    if (!empty($creator['name'])) {
      if (!empty($creator['uri'])) {
        $creator_xml .= <<<CR_URI
  <name type="personal" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="${creator['uri']}">
CR_URI;
      }
      else {
        $creator_xml .= '
  <name type="personal" authority="local">';
      }
      $creator_xml .= <<<CXML

    <namePart>${creator['name']}</namePart>
    <role>
      <roleTerm type="text" authority="marcrelator">Creator</roleTerm>
    </role>
  </name>
CXML;
    }
  }

  // This variable is pretty sneaky, it contains more than the date. We
  // initialize the variable here with the start of mods:originInfo.
  $date_xml = '
  <originInfo>';

  // This generates MODS records for an entire "scrapbook object" - i.e., a book
  // or compound object.
  if ($type !== 'page') {

    // Collections do not require a physical description.
    if ($type !== 'collection') {
      $extent_xml = <<<PDESCRIP
  <physicalDescription>
    <form authority="marcform">print</form>
    <form authority="marcform">electronic</form>
    <extent>${mods_data['extent']}</extent>
    <digitalOrigin>reformatted digital</digitalOrigin>
    <internetMediaType>image/tiff</internetMediaType>
  </physicalDescription>
PDESCRIP;
    }

    $date_xml .= '
    <place>
      <placeTerm type="text" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79007751">New York (N.Y.)</placeTerm>
    </place>
    <place>
      <placeTerm type="code" authority="marccountry">nyu</placeTerm>
    </place>
    <publisher>Online collection published by Barnard Archives and Special Collections, Barnard College.</publisher>';

    $description_xml = <<<ABSTRACT
  <abstract>${mods_data['description']}</abstract>
ABSTRACT;

    $shelf_location = !empty($mods_data['location']) ? "
    <shelfLocator>${mods_data['location']}</shelfLocator>" : '';

    $location_xml = <<<LOCATION_LOCATION_LOCATION
  <location displayLabel="not a PURL">
    <physicalLocation authority="marcorg">NNC</physicalLocation>
    <physicalLocation authority="marcorg">NyNyBAW</physicalLocation>
    <physicalLocation>Barnard College Library (New York, N.Y.)</physicalLocation>{$shelf_location}
  </location>
LOCATION_LOCATION_LOCATION;

    $language_xml = '
  <language>
    <languageTerm type="text">English</languageTerm>
    <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
  </language>';

    $access_condition_xml = '
  <accessCondition type="useAndReproduction">Contact the Archives at archives@barnard.edu for more information on access and use.</accessCondition>';

    $record_info_xml = '
  <recordInfo>
    <recordContentSource>Barnard Archives and Special Collections</recordContentSource>
    <recordContentSource authority="marcorg">NNC</recordContentSource>
    <recordContentSource authority="marcorg">NyNyBAW</recordContentSource>
    <languageOfCataloging>
      <languageTerm type="text" authority="iso639-2b">English</languageTerm>
      <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
    </languageOfCataloging>
  </recordInfo>';
  }

  // Dates - mods:originInfo/date.
  // Approx Date range.
  if (!empty($mods_data['date_start_approx']) && !empty($mods_data['date_end_approx'])) {
    $date_xml .= <<<DATESTR

    <dateCreated encoding="w3cdtf" keyDate="yes" point="start" qualifier="approximate">${mods_data['date_start_approx']}</dateCreated>
    <dateCreated encoding="w3cdtf" point="end" qualifier="approximate">${mods_data['date_end_approx']}</dateCreated>
  </originInfo>
DATESTR;
  }

  // Exact Date range.
  elseif (!empty($mods_data['date_start_exact']) && !empty($mods_data['date_end_exact'])) {
    $date_xml .= <<<DATESTR

    <dateCreated encoding="w3cdtf" keyDate="yes" point="start">${mods_data['date_start_exact']}</dateCreated>
    <dateCreated encoding="w3cdtf" point="end">${mods_data['date_end_exact']}</dateCreated>
  </originInfo>
DATESTR;
  }

  // Exact date, single.
  else {
    $date_xml .= <<<DATESTR

    <dateCreated encoding="w3cdtf" keyDate="yes">${mods_data['date_exact']}</dateCreated>
  </originInfo>
DATESTR;
  }

  // Everything but collections need to have a related collection and PID (duh).
  if ($type !== "collection") {
    $related_item_xml .= <<<EOT

  <relatedItem type="host">
    <titleInfo>
      <title>${mods_data['related_item_title']}</title>
    </titleInfo>
    <identifier type="local">${mods_data['related_item_pid']}</identifier>
    <typeOfResource>mixed material</typeOfResource>
  </relatedItem>
EOT;

    $resource_type_xml = <<<TYPE
  <typeOfResource>{$mods_data['resource_type']}</typeOfResource>
TYPE;

    $genre_xml = <<< GNR
  <genre authority="aat" authorityURI="http://vocab.getty.edu/aat" valueURI="{$mods_data['genre_uri']}">${mods_data['genre']}</genre>
GNR;

    // Topics - mods:subject.
    // Static header (single quote, no eval)
    $subject_xml = '
  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
      <namePart>Barnard College</namePart>
    </name>
  </subject>';
    // Subject terms (requires both a topic and a topic uri).
    $subjects = $mods_data['topics'];
    foreach ($subjects as $topic) {
      if (!empty($topic['topic']) && !empty($topic['uri'])) {
        $subject_xml .= <<<SUBJ

  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="${topic['uri']}">
    <topic>${topic['topic']}</topic>
  </subject>
SUBJ;
      }
    }
  }

  // Build the final return mods record. Better hope you intended everything.
  $mods_xml = <<<MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">

{$identifier_xml}
{$titleinfo_title_xml}
{$creator_xml}
{$resource_type_xml}
{$genre_xml}
{$date_xml}
{$extent_xml}
{$description_xml}
{$location_xml}
{$language_xml}
{$subject_xml}
{$related_item_xml}
{$record_info_xml}
{$access_condition_xml}

</mods>
MODS;

  // Normalize (correct xml formatting if necessary).
  $ret = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $mods_xml);

  return $ret;
}

/**
 * Generates Scrapbook MODS records: RG: BC11-08.
 *
 * @author Ben Rosner, all blame to: brosner@barnard.edu
 *
 * CSV Row processor for Barnard College Alumnae Scrapbooks.
 * The formatting of this section is purposeful.
 * Pay attention to spacing in the creation of variables.
 *
 * We use single quote literals for static content, and heredoc strings for
 * dynamic content. We also use heredoc to gen the final MODs string.
 */
function _bc_islandora_rg1108_mods($mods_data, $type) {
  $identifier_xml = <<<PID
  <identifier type="local">${mods_data['id']}</identifier>
PID;

  $titleinfo_title_xml = <<<TITLE
  <titleInfo>
    <title>${mods_data['title']}</title>
  </titleInfo>
TITLE;

  $resource_type_xml = <<<'TYPE'
  <typeOfResource>text</typeOfResource>
TYPE;

  $genre_xml = <<<'GENRE'
  <genre authority="aat" authorityURI="http://vocab.getty.edu/aat" valueURI="http://vocab.getty.edu/aat/300026657">periodicals</genre>
GENRE;

  // Topics - mods:subject.
  $subject_xml = <<<'BLANKETSUBJ'
  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
      <namePart>Barnard College</namePart>
    </name>
  </subject>
  <subject>
    <name type="corporate" authority="local">
      <namePart>Barnard College. Office of Alumnae Relations.</namePart>
    </name>
  </subject>
  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n84099300">
      <namePart>Barnard College. Associate Alumnae</namePart>
    </name>
  </subject>

  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85141093">
    <topic>Universities and colleges--Alumni and alumnae</topic>
  </subject>
  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85028302">
    <topic>College graduates</topic>
  </subject>
BLANKETSUBJ;

  // Origin info, loc, date, etc.
  $origin_info_xml = <<<'ORIGINSTART'
  <originInfo>
ORIGINSTART;

  if (!empty($mods_data['date_start_approx']) && !empty($mods_data['date_end_approx'])) {
    $origin_date_info_xml = <<<DATESTR
    <dateCreated encoding="w3cdtf" keyDate="yes" point="start" qualifier="approximate">${mods_data['date_start_approx']}</dateCreated>
    <dateCreated encoding="w3cdtf" point="end" qualifier="approximate">${mods_data['date_end_approx']}</dateCreated>
  </originInfo>
DATESTR;
  }
  elseif (!empty($mods_data['date_start_exact']) && !empty($mods_data['date_end_exact'])) {
    // Exact Date range.
    $origin_date_info_xml = <<<DATESTR
    <dateCreated encoding="w3cdtf" keyDate="yes" point="start">${mods_data['date_start_exact']}</dateCreated>
    <dateCreated encoding="w3cdtf" point="end">${mods_data['date_end_exact']}</dateCreated>
  </originInfo>
DATESTR;
  }
  else {
    // Exact date, single.
    $origin_date_info_xml = <<<DATESTR
    <dateCreated encoding="w3cdtf" keyDate="yes">${mods_data['date_exact']}</dateCreated>
  </originInfo>
DATESTR;
  }

  $related_item_xml = <<<'RELATEDITEM'
  <relatedItem type="host" displayLabel="Record Group">
    <titleInfo>
      <title>Administrative Publications</title>
    </titleInfo>
  </relatedItem>
  <relatedItem type="host">
    <titleInfo>
      <title>Barnard Magazine</title>
    </titleInfo>
    <identifier type="local">BC11-08</identifier>
    <typeOfResource>mixed material</typeOfResource>
  </relatedItem>
RELATEDITEM;

  if ($type !== 'page') {

    $origin_info_xml .= <<<'ORIGININFO'
   <place>
     <placeTerm type="text" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79007751">New York (N.Y.)</placeTerm>
   </place>
   <place>
     <placeTerm type="code" authority="marccountry">nyu</placeTerm>
   </place>
   <publisher>Online collection published by Barnard Archives and Special Collections, Barnard College.</publisher>
ORIGININFO;

    $extent_xml = <<<PDESCRIP
  <physicalDescription>
    <form authority="marcform">print</form>
    <form authority="marcform">electronic</form>
    <extent>${mods_data['extent']}</extent>
    <digitalOrigin>reformatted digital</digitalOrigin>
    <internetMediaType>image/jp2</internetMediaType>
  </physicalDescription>
PDESCRIP;

    $creator_xml = <<<'CREATE'
  <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
    <namePart>Barnard College</namePart>
     <role>
        <roleTerm type="text" authority="marcrelator">Publisher</roleTerm>
    </role>
  </name>
CREATE;

    $description_xml = <<<ABSTRACT
  <abstract>${mods_data['description']}</abstract>
ABSTRACT;

    $language_xml = <<<'LANG'
  <language>
    <languageTerm type="text">English</languageTerm>
    <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
  </language>
LANG;

    $access_condition_xml = <<<ACCESSCONDITION
  <accessCondition type="useAndReproduction">${mods_data['useAndReproduction']}</accessCondition>
  <accessCondition type="local rights statements">${mods_data['accesscondition_local']}</accessCondition>
ACCESSCONDITION;

    $record_info_xml = <<<'RSOURCE'
  <recordInfo>
    <recordContentSource>Barnard Archives and Special Collections</recordContentSource>
    <recordContentSource authority="marcorg">NNC</recordContentSource>
    <recordContentSource authority="marcorg">NyNyBAW</recordContentSource>
    <languageOfCataloging>
      <languageTerm type="text" authority="iso639-2b">English</languageTerm>
      <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
    </languageOfCataloging>
  </recordInfo>
RSOURCE;
  }


  // Build the final return mods record. Better hope you intended everything.
  $mods_xml = <<<MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">

{$identifier_xml}
{$titleinfo_title_xml}
{$creator_xml}
{$resource_type_xml}
{$genre_xml}
{$origin_info_xml}
{$origin_date_info_xml}
{$extent_xml}
{$description_xml}
{$language_xml}
{$subject_xml}
{$related_item_xml}
{$record_info_xml}
{$access_condition_xml}

</mods>
MODS;

  // Normalize (correct xml formatting if necessary).
  $ret = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $mods_xml);

  return $ret;
}


/**
 * Generates newspaper issue metadata.
 */
function _bc_islandora_drush_np_issue_mods($title, $extent, $date, $date_short) {
  return <<<ISSUE_MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">
  <titleInfo>
    <title>$title</title>
  </titleInfo>
  <genre authority="marcgt">newspaper</genre>
  <physicalDescription>
    <form authority="marcform">print</form>
    <form authority="marcform">electronic</form>
    <extent>$extent pages</extent>
    <internetMediaType>image/tiff</internetMediaType>
    <digitalOrigin>reformatted digital</digitalOrigin>
  </physicalDescription>
  <relatedItem type="host">
    <titleInfo>
      <title>Barnard Bulletin</title>
    </titleInfo>
    <typeOfResource>text</typeOfResource>
    <identifier type="local">BC12-03</identifier>
  </relatedItem>
  <originInfo>
    <dateIssued encoding="w3cdtf">$date</dateIssued>
    <dateCreated encoding="w3cdtf">$date</dateCreated>
  </originInfo>
  <identifier type="local">BC12-03_$date_short</identifier>
</mods>
ISSUE_MODS;
}

/**
 * Generates (book, newspaper, or publication) page metadata.
 */
function _bc_islandora_drush_page_mods($type, $title, $genre, $host_title, $host_id, $page_id, $date_created, $date_issued) {
  switch ($type) {
    case 'pub':
      $mods_xml = <<<PUB_PAGE_MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <titleInfo>
    <title>$title</title>
  </titleInfo>
  <genre authority="lcgft" authorityURI="http://id.loc.gov/authorities/genreForms" valueURI="http://id.loc.gov/authorities/genreForms/gf2014026139">$genre</genre>
  <relatedItem type="host" displayLabel="Record Group">
    <titleInfo>
      <title>Student Publications</title>
    </titleInfo>
  </relatedItem>
  <relatedItem type="host">
    <titleInfo>
      <title>$host_title</title>
    </titleInfo>
    <typeOfResource>text</typeOfResource>
    <identifier type="local">$host_id</identifier>
  </relatedItem>
  <identifier type="local">$page_id</identifier>
  <originInfo>
    <dateCreated encoding="w3cdtf">$date_created</dateCreated>
  </originInfo>
</mods>
PUB_PAGE_MODS;
      break;

    case 'newspaper':
    case 'book':
    default:
      $mods_xml = <<<DEFAULT_PAGE_MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <titleInfo>
    <title>$host_title</title>
  </titleInfo>
  <genre authority="marcgt">$genre</genre>
  <originInfo>
    $date_issued
    $date_created
  </originInfo>
  <relatedItem type="host">
    <titleInfo>
      <title>$title</title>
    </titleInfo>
    <typeOfResource>text</typeOfResource>
    <identifier type="local">$host_id</identifier>
  </relatedItem>
  <identifier type="local">$page_id</identifier>
</mods>
DEFAULT_PAGE_MODS;
      break;
  }
  return $mods_xml;
}

/**
 * Generates book metadata.
 */
function _bc_islandora_drush_book_mods($title, $date, $extent, $description, $id_date) {
  return <<<BOOK_MODS
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd">
  <titleInfo>
    <title>$title</title>
  </titleInfo>

  <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
    <namePart>Barnard College</namePart>
    <role>
      <roleTerm type="text" authority="marcrelator">Publisher</roleTerm>
    </role>
  </name>

  <typeOfResource>text</typeOfResource>

  <genre authority="marcgt">yearbook</genre>

  <originInfo eventType="publication">
    <place>
      <placeTerm type="code" authority="marccountry">nyu</placeTerm>
      <placeTerm type="text">New York (N.Y.)</placeTerm>
    </place>
    <publisher>Barnard College</publisher>
    <dateIssued encoding="iso8601" keyDate="yes">$date</dateIssued>
    <dateCreated encoding="iso8601" keyDate="yes">$date</dateCreated>
    <issuance>continuing</issuance>
  </originInfo>

  <language>
    <languageTerm type="code" authority="iso639-2b">eng</languageTerm>
  </language>

  <physicalDescription>
    <form authority="marcform">print</form>
    <form authority="marcform">electronic</form>
    <extent>$extent</extent>
    <internetMediaType>image/jpeg2000</internetMediaType>
    <digitalOrigin>reformatted digital</digitalOrigin>
  </physicalDescription>

  <abstract>$description</abstract>

  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85147738">
    <topic>Women's colleges</topic>
  </subject>
  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85147501">
    <topic>Women college students</topic>
  </subject>

  <subject authority="lcsh" authorityURI="http://id.loc.gov/authorities/subjects" valueURI="http://id.loc.gov/authorities/subjects/sh85129244">
    <topic>Student activities</topic>
  </subject>

  <subject>
    <name type="corporate" authority="naf" authorityURI="http://id.loc.gov/authorities/names" valueURI="http://id.loc.gov/authorities/names/n79065105">
    </name>
  </subject>

  <relatedItem type="host">
    <titleInfo>
      <title>Mortarboard</title>
    </titleInfo>
    <typeOfResource>text</typeOfResource>
    <identifier type="local">BC12-01</identifier>
  </relatedItem>

  <identifier type="local">BC12-01_$id_date</identifier>

  <location>
    <physicalLocation authority="marcorg">NNC</physicalLocation>
    <physicalLocation authority="marcorg">NyNyBAW</physicalLocation>
    <physicalLocation>Barnard College Library (New York, N.Y.)</physicalLocation>
  </location>

  <accessCondition type="useAndReproduction">This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. Contact the Archives at archives@barnard.edu for more information on access and use, including commercial uses.</accessCondition>

  <recordInfo>
    <recordContentSource>Barnard Archives and Special Collections</recordContentSource>
    <recordContentSource authority="marcorg">NNC</recordContentSource>
    <recordContentSource authority="marcorg">NyNyBAW</recordContentSource>
    <languageOfCataloging>
      <languageTerm authority="iso639-2b">eng</languageTerm>
    </languageOfCataloging>
  </recordInfo>
</mods>
BOOK_MODS;
}

/**
 * Generates Barnard Manupscript and Scrapbook MODS MD DS.
 */
function _bc_islandora_ms_mods($ms_data) {
  if (isset($ms_data['type']) && $ms_data['type'] == 'page') {
    $date_xml = '<originInfo>';
  }
  else {
    $date_xml = "<originInfo>
    <place>
      <placeTerm type=\"text\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"http://id.loc.gov/authorities/names/n79007751\">New York (N.Y.)</placeTerm>
    </place>
    <place>
      <placeTerm type=\"code\" authority=\"marccountry\">nyu</placeTerm>
    </place>
    <publisher>Online collection published by Barnard Archives and Special Collections, Barnard College.</publisher>";
    $extent_xml = "
  <physicalDescription>
    <form authority=\"marcform\">print</form>
    <form authority=\"marcform\">electronic</form>
    <extent>{$ms_data['extent']}</extent>
    <digitalOrigin>reformatted digital</digitalOrigin>
    <internetMediaType>image/tiff</internetMediaType>
  </physicalDescription>";
    $description_xml = "
  <abstract>{$ms_data['description']}</abstract>";
    $location_xml = "
  <location displayLabel=\"not a PURL\">
    <physicalLocation authority=\"marcorg\">NNC</physicalLocation>
    <physicalLocation authority=\"marcorg\">NyNyBAW</physicalLocation>
    <physicalLocation>Barnard College Library (New York, N.Y.)</physicalLocation>
    <shelfLocator>{$ms_data['location']}</shelfLocator>
  </location>";
    $language_xml = "
  <language>
    <languageTerm type=\"text\">English</languageTerm>
    <languageTerm type=\"code\" authority=\"iso639-2b\">eng</languageTerm>
  </language>";
    $access_condition_xml = "
  <accessCondition type=\"useAndReproduction\">Contact the Archives at archives@barnard.edu for more information on access and use.</accessCondition>";
    $record_info_xml = "
  <recordInfo>
    <recordContentSource>Barnard Archives and Special Collections</recordContentSource>
    <recordContentSource authority=\"marcorg\">NNC</recordContentSource>
    <recordContentSource authority=\"marcorg\">NyNyBAW</recordContentSource>
    <languageOfCataloging>
      <languageTerm type=\"text\" authority=\"iso639-2b\">English</languageTerm>
      <languageTerm type=\"code\" authority=\"iso639-2b\">eng</languageTerm>
    </languageOfCataloging>
  </recordInfo>";
  }

  // @INFO Note that the first creator variable has no number.
  $creators = ['creator', 'creator1', 'creator2', 'creator3'];
  $creator_xml = "";
  foreach ($creators as $creator) {
    if (!empty($ms_data[$creator])) {
      $creator_xml .= isset($ms_data[$creator . '_uri']) && !empty($ms_data[$creator . '_uri']) ?
        "<name type=\"personal\" authority=\"naf\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"{$ms_data[$creator.'_uri']}\">" :
        "<name type=\"personal\" authority=\"local\">";
      $creator_xml .= "
      <namePart>{$ms_data[$creator]}</namePart>
      <role>
        <roleTerm type=\"text\" authority=\"marcrelator\">Creator</roleTerm>
      </role>\n";
      $creator_xml .= "</name>\n";
    }
  }

  if (!empty($ms_data['date_start']) && !empty($ms_data['date_end'])) {
    $date_xml .= "
    <dateCreated encoding=\"w3cdtf\" keyDate=\"yes\" point=\"start\" qualifier=\"approximate\">{$ms_data['date_start']}</dateCreated>
    <dateCreated encoding=\"w3cdtf\" point=\"end\" qualifier=\"approximate\">{$ms_data['date_end']}</dateCreated>";
  }
  else {
    $date_xml .= "
    <dateCreated encoding=\"w3cdtf\" keyDate=\"yes\">{$ms_data['date']}</dateCreated>";
  }
  $date_xml .= "
  </originInfo>";
  $mods_xml = "<mods xmlns=\"http://www.loc.gov/mods/v3\" xmlns:mods=\"http://www.loc.gov/mods/v3\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xsi:schemaLocation=\"http://www.loc.gov/mods/v3 http://www.loc.gov/mods/v3/mods-3-5.xsd\">
  <identifier type=\"local\">{$ms_data['id']}</identifier>
  <titleInfo>
    <title>{$ms_data['title']}</title>
  </titleInfo>
  {$creator_xml}
  <typeOfResource>{$ms_data['resource_type']}</typeOfResource>
  <genre authority=\"aat\" authorityURI=\"http://vocab.getty.edu/aat\" valueURI=\"{$ms_data['genre_uri']}\">{$ms_data['genre']}</genre>
  {$date_xml}";
  if (isset($extent_xml)) {
    $mods_xml .= "$extent_xml";
  }
  if (isset($description_xml)) {
    $mods_xml .= "$description_xml";
  }
  if (isset($location_xml)) {
    $mods_xml .= "$location_xml";
  }
  $mods_xml .= "
  <subject authority=\"lcsh\" authorityURI=\"http://id.loc.gov/authorities/subjects\" valueURI=\"{$ms_data['topic_uri']}\">
    <topic>{$ms_data['topic']}</topic>
  </subject>";
  if (isset($language_xml)) {
    $mods_xml .= "{$language_xml}";
  }
  // Updated for S&F.
  if (isset($ms_data['type']) && $ms_data['type'] == 'page') {
    $mods_xml .= "
  <subject>
    <name type=\"corporate\" authority=\"naf\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"http://id.loc.gov/authorities/names/n79065105\">
      <namePart>Barnard College</namePart>
    </name>
  </subject>
  <subject>
    <name type=\"corporate\" authority=\"naf\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"http://id.loc.gov/authorities/names/no90023871\">
      <namePart>Barnard Center for Research on Women</namePart>
    </name>
  </subject>
  <subject authority=\"lcsh\" authorityURI=\"http://id.loc.gov/authorities/subjects\" valueURI=\"http://id.loc.gov/authorities/subjects/sh2008001560\">
    <topic>Second-wave feminism</topic>
  </subject>
  ";
  }
  else {
    $mods_xml .= "
  <subject>
    <name type=\"corporate\" authority=\"naf\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"http://id.loc.gov/authorities/names/n79065105\">
        <namePart>Barnard College</namePart>
    </name>
  </subject>
  <subject>
    <name type=\"corporate\" authority=\"naf\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"http://id.loc.gov/authorities/names/no90023871\">
        <namePart>Barnard Center for Research on Women</namePart>
    </name>
  </subject>
  <subject>
    <name type=\"corporate\" authority=\"naf\" authorityURI=\"http://id.loc.gov/authorities/names\" valueURI=\"http://id.loc.gov/authorities/names/n98110944\">
        <namePart>Scholar and the Feminist Conference</namePart>
    </name>
  </subject>
  <subject authority=\"lcsh\" authorityURI=\"http://id.loc.gov/authorities/subjects\" valueURI=\"http://id.loc.gov/authorities/subjects/sh2008001560\">
    <topic>Second-wave feminism</topic>
  </subject>
  <subject authority=\"lcsh\" authorityURI=\"http://id.loc.gov/authorities/subjects\" valueURI=\"http://id.loc.gov/authorities/subjects/sh85083267\">
    <topic>Meetings</topic>
  </subject>
  <subject authority=\"lcsh\" authorityURI=\"http://id.loc.gov/authorities/subjects\" valueURI=\"http://id.loc.gov/authorities/subjects/sh85047741\">
    <topic>Feminism</topic>
  </subject>
  ";
  }
  $mods_xml .= "
  <relatedItem type=\"host\" displayLabel=\"Record Group\">
    <titleInfo>
      <title>Academic Departments and Programs</title>
    </titleInfo>
  </relatedItem>
  <relatedItem type=\"host\">
    <titleInfo>
      <title>Barnard Center for Research on Women</title>
    </titleInfo>
    <identifier type=\"local\">BC13-58</identifier>
    <typeOfResource>mixed material</typeOfResource>
  </relatedItem>
  ";

  if (isset($access_condition_xml)) {
    $mods_xml .= "{$access_condition_xml}";
  }
  if (isset($record_info_xml)) {
    $mods_xml .= "{$record_info_xml}";
  }
  $mods_xml .= "\n</mods>";
  return $mods_xml;
}

