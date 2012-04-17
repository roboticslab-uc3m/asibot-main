# Copyright: Universidad Carlos III de Madrid (C) 2010
# Authors: Juan G. Victores

add_subdirectory(CartesianClient)
add_subdirectory(CartesianServer)
add_subdirectory(TrajGen)  # order important here
# rlPlugins a plugin library which actually contains some YARP devices
add_subdirectory(rlPlugins)

