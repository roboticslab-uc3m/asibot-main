/*
 * Copyright: (C) 2013 UC3M ; (C) 2010 RobotCub Consortium
 * Author: Juan G Victores
 * Contrib: Lorenzo Natale, Paul Fitzpatrick ((authors of YARP equivalent))
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

//run this to load the asibot library and initialize the network in Java

//import yarp.DVector;

class LoadAsibot{
	private static int asibotInitialized=0;

	public static int isLoaded()
	{ return asibotInitialized; }

    public LoadAsibot() {
		if (asibotInitialized==0)
			{
				System.loadLibrary("jasibot");
				System.out.println("Asibot library loaded and initialized");
				System.out.flush();
			}
		else
			{
				System.out.println("Asibot library already loaded and initialized, doing nothing");
				System.out.flush();
			}
			
		asibotInitialized=1;
    }
}

