        ��  ��                  �      �� ��     0 	        �4   V S _ V E R S I O N _ I N F O     ���             ?                            S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0   j %  C o m p a n y N a m e     L o s   A l a m o s   N a t i o n a l   L a b o r a t o r y   ( D - 3 )     p $  F i l e D e s c r i p t i o n     C o G r a n d I m p o r t   L i b r a r y   -   C O M   P l u g   I n   R   F i l e V e r s i o n     V e r s i o n   2 . 2 . 0 . 1   f o r   B 2 R 1     <   I n t e r n a l N a m e   C o G r a n d I m p o r t   � 4  L e g a l C o p y r i g h t   C o p y r i g h t   �   L A N S ,   L L C .     N e a r l y   a l l   r i g h t s   r e s e r v e d .   L   O r i g i n a l F i l e n a m e   C o G r a n d I m p o r t . D L L   <   P r o d u c t N a m e     C o G r a n d I m p o r t   X   P r o d u c t V e r s i o n   V e r s i o n     2 . 2 . 0 . 1   f o r   B 2 R 1   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�u  0   R E G I S T R Y   ��g       0 	        HKCR
{
	Integrated.Review.System
	{
		Import
		{
			CoGrandImport.GrandImport.1 = s 'GrandImport Class'
			{
				CLSID = s '{BD000CB2-05DF-11D6-8176-00C04F60E89B}'
			}
			CoGrandImport.GrandImport = s 'GrandImport Class'
			{
				CLSID = s '{BD000CB2-05DF-11D6-8176-00C04F60E89B}'
				CurVer = s 'CoGrandImport.GrandImport.1'
				ImportFileType = s 'BID, BI0'
				InstrumentName = s 'GRAND'
			}
		}
	}
	NoRemove CLSID
	{
		ForceRemove {BD000CB2-05DF-11D6-8176-00C04F60E89B} = s 'GrandImport Class'
		{
			ProgID = s 'Integrated.Review.System\Import\CoGrandImport.GrandImport.1'
			VersionIndependentProgID = s 'Integrated.Review.System\Import\CoGrandImport.GrandImport'
			ForceRemove 'Programmable'
			InprocServer32 = s '%MODULE%'
			{
				val ThreadingModel = s 'Apartment'
			}
			'TypeLib' = s '{BD000CA1-05DF-11D6-8176-00C04F60E89B}'
		}
	}
}
   �  ,   T Y P E L I B   ��     0 	        MSFT      	      A                         =   �      ����       �             d   L  �   ����   d     ����   p     ����   T     ����     �   ����   �  �   ����   �     ����   �     ����   �  �  ����   0  0   ����   ����    ����   `  T   ����   �  $   ����   ����    ����   ����    ����   %"  �                                     `                         ����                  ����4" �                                    �   @  4       4           ���� t             �������������   ��������������������������������������������    ������������x   ��������������������H      ������������`   �   0   � ����v �O`���������e�w�|Q���  �w<���������c�w�|Q���  �w<���������d�w�|Q���  �w<���������� ����v �O`�    ����0     �      F   ����      �      F   ����� ����v �O`�d   ����d      ��������      �   x          - stdole2.tlbWWW����p  �����  �����   �   �����������������  ��������@     �    ����   ���������  ����������������������������d  �  ������������    ��������\  ���������������������������������   ����    4   ���������  <  �����  T  �����  P  �������������  ��������  ����X  ����   ��������x  <  ���������  ����h  �����  ��������<  �����  �  ����������������������������$  �����  \  (  �  ������������x   �  ���������  �  ����������������`   L   ����  ����|  �  ����  ���������  �������� ��COGRANDIMPORTLib    ����8H�GrandImportWd   ����83uIGrandImportd   ���� ��AddDataW�������� tssFacilityIDW�������� j�bstrFilename��������
 cbOverwriteWW��������
 � bstrResultWW�������� <�pbResultd   ���� ��BackupDataStoreW�������� �qbstrDestinationPathW��������	 #>bQuestionWWW�������� ��bDefaultd   ���� ֔CloseDataStoreWWd   ���� ��CreateNewDataStoreWWd   ����
 o�DeleteDataWW��������
 ��lStationIDWW�������� �dtStartDateW��������	 �dtEndDateWWW�������� Y�pbStatusd   ���� O�DeleteDataStoreWd   ���� |�GetDataByChannel��������
 �isChannelIDWW��������	 L�StartTimeWWW�����   �7EndTimeW�������� ��pvDataWWd   ���� yGGetDataByStationd   ���� �GetDataStatisticsWWW�������� �TpActualStartTime�������� ��pActualEndTimeWW�������� S�plNumMembersd   ���� ��GetDataStorePath�����   �qbstrStorePathWWWd   ���� [GetDataStoreSize�������� �pdSizeInBytesWWWd   ���� ˞GetDataStoreStatusWW����$   �plStatusd   x   ��GetDataStoreTimeRangeWWW�������� ��bErrDisplayW�����    �BFirstTimestampWW�������� D�LastTimestampWWWd   ���� ��GetDayStatisticsd   ���� 7�GetDaySummaryDataWWW�������� '�pvDayWWWd   ���� �GetFilesList�������� ��bstrRootDirW�������� A�bIncludeSubDirsW�����    �XbMostRecentW�������� 6PpsaFilesListd   �   ��GetLastError�������� ��pbstrErrortextWW�������� �plErrorWd   ���� ��GetParametersWWW�������� ��pvParmsWd   ���� ��InitializeDataStoreW��������	 f�pbSuccessWWWd   ���� zOpenDataStoreWWWd   ���� LRestoreDataStore����4   �bstrSourcePathWWd   ���� �SetParametersWWW�������� $vParmsWW CoGrandImport 1.0 Type Library GrandImport ClassW IGrandImport Interface method AddData method BackupDataStore method CloseDataStoreW method CreateNewDataStoreW method DeleteDataW method DeleteDataStore method GetDataByChannelWWW method GetDataByStationWWW method GetDataStatisticsWW method GetDataStorePathWWW method GetDataStoreSizeWWW method GetDataStoreStatusW method GetDataStoreTimeRangeWW method GetDayStatisticsWWW method GetDaySummaryDataWW method GetFilesListWWW method GetLastErrorWWW method GetParametersWW method InitializeDataStore method OpenDataStoreWW method RestoreDataStoreWWW method SetParametersWW @ � @ � @ � @ � @ � @ � >   Created by MIDL version 6.00.0347 at Wed Nov 12 09:27:07 2014
 \�cTWW [ WW       ����0   D       H   L        \    �     � 	D         L    �`       �x       ��          �         �   
   \   �      � 	D        \    �`       ��       �      �(        �   
       �    $ 4 	         t   8   �    ( \ 	D        �    �`         �   
   \   �    , � 	D        �    �`       ��      ��      ��        �  
   8   �    0 \ 	D        �    �`         �  
   t   �    4 � 	D        �    �`       ��      �$      �<      �T        h        �   
   h   �    8 � 	D        �    �`       ��      �<      �T        h        �   
   �   �    < � 	D 	          �`       ��      �$      �<      �T        �        �         �        �   
   8 	  �    @ \ 	D	        (   �`          $  
   D 
  �    D t 	D
        D   �`      (   \        �   
   8   �    H \ 	D        `   �`          �  
   h   �    L � 	D        |   �`       ��      ��        �                �   
   �   �    P � 	D        �   �`       ��      �<      �T        �        �         �        �   
   h   �    T � 	D        �   �`       ��      �<      �T        \        �   
   \   �    X � 	        �   �`       ��      ��      ��        �     D   �    \ t 	D        �   �`                      
   D   �    ` t 	D           �`         P        �   
   D   �    d l 	D           �`       �        �  
   8   �    h \ 	D        8   �`         �  
   \   �    l � 	D        P   �`       ��      �      �(        �   
   D   �    p l 	D        l   �`       �        �   
                           	   
                                       L   �   <  X  x  �    |  �    @  x  �     <  p  �  4  d  �  �  �      \   �   �     l  �    �    D  �  �  (  �    l  �  �  8  p  �  :       �� ��     0	                 C o G r a n d I m p o r t                         