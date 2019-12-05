; Run this from within the sites/all directory:
; drush make --yes --no-core --contrib-destination=. bc_oralhistory.drush.make

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
# projects[islandora_solution_pack_oralhistories][download][url] = "git@github.com:Islandora-Labs/islandora_solution_pack_oralhistories.git"
# projects[islandora_solution_pack_oralhistories][download][branch] = "master"
# projects[islandora_solution_pack_oralhistories][download][tag]  = "7.x-1.12"

# projects[bc_islandora][download][url] = "git@github.com:BarnardArchives/bc_islandora.git"
# projects[bc_islandora][download][branch] = "master"
# projects[bc_islandora][download][tag]  = ""

projects[barnard_islandora_drush][download][url] = "git@github.com:BarnardArchives/barnard_islandora_drush.git"
projects[barnard_islandora_drush][subdir] = "drush"
projects[barnard_islandora_drush][download][branch] = "master"
projects[barnard_islandora_drush][download][tag]  = ""
