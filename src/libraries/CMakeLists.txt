# Copyright: Universidad Carlos III de Madrid (C) 2010
# Authors: Juan G. Victores

add_subdirectory(TinyMath)  # must be at the beginning
add_subdirectory(TrajGen)  # must be at the beginning

add_subdirectory(CartesianClientLib)
add_subdirectory(CartesianServerLib)
add_subdirectory(ColorClientLib)
add_subdirectory(RlPlugins)  # RlPlugins a plugin library which actually contains some YARP plugins (devices)

add_subdirectory(TaskLib) # must be towards the end

#IF (ENABLE_TravisLib)
#    add_subdirectory(TravisLib)
#ENDIF (ENABLE_TravisLib)

