
local_mod = $(notdir $(CURDIR))

# for ext_mod_list
include _00_Ext_Mods.mak
# for mod_all, it includes children
include mod_all.mak

base_mod_dir = ../../mod
mod_include_dir = include
mod_archive_dir = archive

all_mod_folders = $(patsubst %, $(base_mod_dir)/%, $(ext_mod_list))

all_mod_include_paths = $(patsubst %, %/$(mod_include_dir), $(all_mod_folders))
# For compile
all_mod_includes      = $(foreach p, $(all_mod_include_paths), -I$p)

#all_mod_archive_paths = $(patsubst %, %/$(mod_archive_dir), $(all_mod_folders))
# For make
all_mod_all_level_archives = $(foreach p, $(mod_all), $(base_mod_dir)/$p/$(mod_archive_dir)/$p.a)
