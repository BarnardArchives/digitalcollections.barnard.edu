; Run this from within the sites/all directory:
; drush make --yes --no-core --contrib-destination=. bc_theme.drush.make

; Core version
core = 7.x

; Should always be 2.
api = 2

; Defaults that apply to all modules.
defaults[projects][type] = "module"
defaults[projects][download][type] = "git"
defaults[projects][download][overwrite] = TRUE
defaults[projects][subdir] = ""

# ; Barnard Archives
# projects[bootstrap][download][url] = "https://git.drupalcode.org/project/bootstrap.git"
# projects[bootstrap][type] = "theme"
# projects[bootstrap][download][branch] = "7.x-3.x"
# projects[bootstrap][download][tag]  = ""

# projects[bc_islandora_exhibit][download][url] = "git@github.com:BarnardArchives/barnard_islandora_exhibit.git"
# projects[bc_islandora_exhibit][download][branch] = "master"
# projects[bc_islandora_exhibit][download][tag]  = ""

projects[barnard_bootstrap][download][url] = "git@github.com:BarnardArchives/barnard_bootstrap.git"
projects[barnard_bootstrap][type] = "theme"
projects[barnard_bootstrap][download][branch] = "master"
projects[barnard_bootstrap][download][tag]  = ""
