# Copyright: Universidad Carlos III de Madrid (C) 2010
# Authors: Juan G. Victores

#add_subdirectory(TestRavePartLib)
add_subdirectory(TestRaveBotLib)
add_subdirectory(CartesianClientLib)
add_subdirectory(CartesianServerLib)
add_subdirectory(ColorClientLib)
add_subdirectory(ExtraMath)  # order important here
add_subdirectory(TrajGen)  # order important here
add_subdirectory(RlPlugins)  # RlPlugins a plugin library which actually contains some YARP devices

