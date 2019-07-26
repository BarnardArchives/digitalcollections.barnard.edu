; Run this from within the sites/all directory:
; drush make --yes --no-core --contrib-destination=. bc_modules.drush.make

; Core version
core = 7.x

; Should always be 2.
api = 2

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
# projects[barnard_islandora_drush][download][url] = "git@github.com:BarnardArchives/barnard_islandora_drush.git"
# projects[barnard_islandora_drush][download][branch] = "master"
# projects[barnard_islandora_drush][download][tag]  = ""
# projects[bc_islandora_exhibit][download][url] = "git@github.com:BarnardArchives/barnard_islandora_exhibit.git"
# projects[bc_islandora_exhibit][download][branch] = "master"
# projects[bc_islandora_exhibit][download][tag]  = ""
# projects[islandora_solr_content_type][download][url] = "git@github.com:BarnardArchives/islandora_solr_content_type.git"
# projects[islandora_solr_content_type][download][branch] = "master"
# projects[islandora_solr_content_type][download][tag]  = ""
# projects[barnard_content_feature][download][url] = "git@github.com:discoverygarden/barnard_content_feature.git"
# projects[barnard_content_feature][download][branch] = "master"
# projects[barnard_content_feature][download][tag]  = ""
# projects[barnard_theme_feature][download][url] = "git@github.com:discoverygarden/barnard_theme_feature.git"
# projects[barnard_theme_feature][download][branch] = "master"
# projects[barnard_theme_feature][download][tag]  = ""
# projects[bc_citation][download][url] = "git@github.com:br2490/bc_citation.git"
# projects[bc_citation][download][branch] = "master"
# projects[bc_citation][download][tag]  = ""
