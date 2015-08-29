/*
 * BackupRegSettings.h
 *
 *  Created on: 29 ���. 2015 �.
 *      Author: Kreyl
 */

#ifndef BACKUPREGSETTINGS_H_
#define BACKUPREGSETTINGS_H_

#define BKPREG_CHECK        BKP->DR1     // Register to store "IsStored" const
#define IsStored()          (BKPREG_CHECK == 0xA5A5)
#define SetStored()         BKPREG_CHECK = 0xA5A5

#define BKPREG_BRIGHTNESS   BKP->DR2


#endif /* BACKUPREGSETTINGS_H_ */
