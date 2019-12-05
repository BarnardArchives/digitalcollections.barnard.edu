; Run this from within the sites/all directory:
; drush make --yes --no-core --contrib-destination=. barnard.drush.make

; Core version
core = 7.x

; Should always be 2.
api = 2

; Modules

; Defaults that apply to all modules.
defaults[projects][type] = "module"
defaults[projects][download][type] = "git"
defaults[projects][download][tag]  = "7.x-1.12"
defaults[projects][download][overwrite] = TRUE
defaults[projects][subdir] = ""

; Islandora
projects[islandora][download][url] = "https://github.com/Islandora/islandora.git"
projects[islandora_batch][download][url] = "https://github.com/Islandora/islandora_batch.git"
projects[islandora_book_batch][download][url] = "https://github.com/Islandora/islandora_book_batch.git"
projects[islandora_bookmark][download][url] = "https://github.com/Islandora/islandora_bookmark.git"
projects[islandora_importer][download][url] = "https://github.com/Islandora/islandora_importer.git"
projects[islandora_internet_archive_bookreader][download][url] = "https://github.com/Islandora/islandora_internet_archive_bookreader.git"
projects[islandora_ip_embargo][download][url] = "https://github.com/Islandora/islandora_ip_embargo.git"
projects[islandora_ip_embargo][download][branch] = "7.x"
projects[islandora_ip_embargo][download][tag] = ""
projects[islandora_videojs][download][url] = "https://github.com/Islandora/islandora_videojs.git"
projects[islandora_marcxml][download][url] = "https://github.com/Islandora/islandora_marcxml.git"
projects[islandora_oai][download][url] = "https://github.com/Islandora/islandora_oai.git"
projects[islandora_ocr][download][url] = "https://github.com/Islandora/islandora_ocr.git"
projects[islandora_openseadragon][download][url] = "https://github.com/Islandora/islandora_openseadragon.git"
projects[islandora_paged_content][download][url] = "https://github.com/Islandora/islandora_paged_content.git"
projects[islandora_simple_workflow][download][url] = "https://github.com/Islandora/islandora_simple_workflow.git"
projects[islandora_solr_metadata][download][url] = "git@github.com:Islandora/islandora_solr_metadata.git"
projects[islandora_solr_search][download][url] = "https://github.com/Islandora/islandora_solr_search.git"
projects[islandora_solr_views][download][url] = "https://github.com/Islandora/islandora_solr_views.git"
projects[islandora_solution_pack_audio][download][url] = "https://github.com/Islandora/islandora_solution_pack_audio.git"
projects[islandora_solution_pack_book][download][url] = "https://github.com/Islandora/islandora_solution_pack_book.git"
projects[islandora_solution_pack_collection][download][url] = "https://github.com/Islandora/islandora_solution_pack_collection.git"
projects[islandora_solution_pack_compound][download][url] = "https://github.com/Islandora/islandora_solution_pack_compound.git"
projects[islandora_solution_pack_document][download][url] = "https://github.com/discoverygarden/islandora_solution_pack_document.git"
projects[islandora_solution_pack_document][download][branch] = "7.x"
projects[islandora_solution_pack_document][download][tag] = ""
projects[islandora_solution_pack_image][download][url] = "https://github.com/Islandora/islandora_solution_pack_image.git"
projects[islandora_solution_pack_large_image][download][url] = "https://github.com/Islandora/islandora_solution_pack_large_image.git"
projects[islandora_solution_pack_pdf][download][url] = "https://github.com/Islandora/islandora_solution_pack_pdf.git"
projects[islandora_solution_pack_video][download][url] = "https://github.com/Islandora/islandora_solution_pack_video.git"
projects[islandora_xacml_editor][download][url] = "https://github.com/Islandora/islandora_xacml_editor.git"
projects[islandora_xml_forms][download][url] = "https://github.com/Islandora/islandora_xml_forms.git"
projects[objective_forms][download][url] = "https://github.com/Islandora/objective_forms.git"
projects[php_lib][download][url] = "https://github.com/Islandora/php_lib.git"
projects[islandora_solution_pack_newspaper][download][url] = "https://github.com/Islandora/islandora_solution_pack_newspaper.git"
projects[islandora_pdfjs][download][url] = "https://github.com/Islandora/islandora_pdfjs.git"
projects[islandora_fits][download][url] = "https://github.com/Islandora/islandora_fits.git"
projects[islandora_scholar][download][url] = "https://github.com/Islandora/islandora_scholar"
projects[islandora_pathauto][download][url] = "https://github.com/Islandora/islandora_pathauto.git"

; Discovery Garden
projects[islandora_jodconverter][download][url] = "https://github.com/discoverygarden/islandora_jodconverter.git"
projects[islandora_jodconverter][download][branch] = "7.x"
projects[islandora_jodconverter][download][tag]  = ""
projects[islandora_ga_reports][download][url] = "https://github.com/discoverygarden/islandora_ga_reports"
projects[islandora_ga_reports][download][branch] = "7.x"
projects[islandora_ga_reports][download][tag]  = ""
projects[islandora_plupload][download][url] = "https://github.com/discoverygarden/islandora_plupload.git"
projects[islandora_plupload][download][branch] = "7.x"
projects[islandora_plupload][download][tag]  = ""
projects[islandora_solution_pack_manuscript][download][url] = "https://github.com/discoverygarden/islandora_solution_pack_manuscript.git"
projects[islandora_solution_pack_manuscript][download][branch] = "7.x"
projects[islandora_solution_pack_manuscript][download][tag]  = ""
projects[google_analytics_reports][download][url] = "http://github.com/discoverygarden/google_analytics_reports.git"
projects[google_analytics_reports][download][branch] = "7.x"
projects[google_analytics_reports][download][tag]  = ""

; Other
projects[islandora_find_replace][download][url] = "https://github.com/contentmath/islandora_find_replace.git"
projects[islandora_find_replace][download][branch] = "master"
projects[islandora_find_replace][download][tag]  = ""
projects[islandora_datastream_crud][download][url] = "https://github.com/mjordan/islandora_datastream_crud.git"
projects[islandora_datastream_crud][download][branch] = "7.x"
projects[islandora_datastream_crud][download][tag]  = ""
projects[islandora_patches][download][url] = "https://github.com/Islandora-Labs/islandora_patches.git"
projects[islandora_patches][download][branch] = "7.x"
projects[islandora_patches][download][tag]  = ""


; Run this from within the sites/all directory:
; drush make --yes --no-core --contrib-destination=. bc_modules.drush.make

; Core version
core = 7.x

; Should always be 2.
api = 2

; Defaults that apply to all modules.
defaults[projects][type] = "module"
defaults[projects][download][type] = "git"
defaults[projects][download][overwrite] = TRUE
defaults[projects][subdir] = ""

; Barnard Archives
projects[islandora_openseadragon_paged][download][url] = "git@github.com:BarnardArchives/islandora_openseadragon_paged.git"
projects[islandora_openseadragon_paged][download][branch] = "master"
projects[islandora_openseadragon_paged][download][tag]  = ""
projects[barnard_compound_bookreader][download][url] = "git@github.com:BarnardArchives/compound_book_reader.git"
projects[barnard_compound_bookreader][download][branch] = "master"
projects[barnard_compound_bookreader][download][tag]  = ""

projects[bc_islandora][download][url] = "git@github.com:BarnardArchives/bc_islandora.git"
projects[bc_islandora][download][branch] = "master"
projects[bc_islandora][download][tag]  = ""
projects[barnard_islandora_drush][download][url] = "git@github.com:BarnardArchives/barnard_islandora_drush.git"
projects[barnard_islandora_drush][download][branch] = "master"
projects[barnard_islandora_drush][download][tag]  = ""
projects[bc_islandora_exhibit][download][url] = "git@github.com:BarnardArchives/barnard_islandora_exhibit.git"
projects[bc_islandora_exhibit][download][branch] = "master"
projects[bc_islandora_exhibit][download][tag]  = ""
projects[islandora_solr_content_type][download][url] = "git@github.com:BarnardArchives/islandora_solr_content_type.git"
projects[islandora_solr_content_type][download][branch] = "master"
projects[islandora_solr_content_type][download][tag]  = ""
# projects[barnard_content_feature][download][url] = "git@github.com:discoverygarden/barnard_content_feature.git"
# projects[barnard_content_feature][download][branch] = "master"
# projects[barnard_content_feature][download][tag]  = ""
# projects[barnard_theme_feature][download][url] = "git@github.com:discoverygarden/barnard_theme_feature.git"
# projects[barnard_theme_feature][download][branch] = "master"
# projects[barnard_theme_feature][download][tag]  = ""
# projects[bc_citation][download][url] = "git@github.com:br2490/bc_citation.git"
# projects[bc_citation][download][branch] = "master"
# projects[bc_citation][download][tag]  = ""

projects[barnard_theme][download][url] = "git@github.com:BarnardArchives/barnard_theme.git"
projects[barnard_theme][type] = "theme"
projects[barnard_theme][download][branch] = "master"
projects[barnard_theme][download][tag]  = ""

projects[bootstrap][download][url] = "https://git.drupalcode.org/project/bootstrap.git"
projects[bootstrap][type] = "theme"
projects[bootstrap][download][branch] = "7.x-3.x"
projects[bootstrap][download][tag]  = ""
projects[barnard_bootstrap][download][url] = "git@github.com:BarnardArchives/barnard_bootstrap.git"
projects[barnard_bootstrap][type] = "theme"
projects[barnard_bootstrap][download][branch] = "master"
projects[barnard_bootstrap][download][tag]  = ""

projects[barnard_islandora_drush][download][url] = "git@github.com:BarnardArchives/barnard_islandora_drush.git"
;; This subdir is wrong. Not sure how to get it to
projects[barnard_islandora_drush][subdir] = "drush"
projects[barnard_islandora_drush][download][branch] = "master"
projects[barnard_islandora_drush][download][tag]  = ""

;; Pretty sure we need to either specify a particular 1.x commit of bookreader, or
;; use https://github.com/Islandora/internet_archive_bookreader
;libraries[bookreader][download][url] = "git://github.com/openlibrary/bookreader.git"
;libraries[bookreader][download][branch] = "UNKNOWN"
;libraries[bookreader][download][tag] = "UNKNOWN"

libraries[tuque][download][url] = "https://github.com/Islandora/tuque.git"
libraries[tuque][download][branch] = "1.x"

libraries[citeproc-php][download][url] = "git@github.com:Islandora/citeproc-php.git"
libraries[citeproc-php][download][branch] = "master"

libraries[videojs][download][type] = "file"
libraries[videojs][download][url] = "https://github.com/videojs/video.js/releases/download/v5.10.2/video-js-5.10.2.zip"
