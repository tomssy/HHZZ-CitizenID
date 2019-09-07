# -*- coding: utf-8 -*-
"""
Created on Thu Apr  5 12:11:17 2018

@author: jxw10
"""
import csv
import os

file_path=os.path.dirname(os.path.realpath(__file__))
file_path=file_path+'\handrecord.csv'
file_coo = open(file_path)
name=['thumb_met_st_pos_x','thumb_met_st_pos_y','thumb_met_st_pos_z','thumb_met_ed_pos_x','thumb_met_ed_pos_y','thumb_met_ed_pos_z','thumb_pro_ed_pos_x','thumb_pro_ed_pos_y','thumb_pro_ed_pos_z','thumb_mid_ed_pos_x','thumb_mid_ed_pos_y','thumb_mid_ed_pos_z','thumb_dis_ed_pos_x','thumb_dis_ed_pos_y','thumb_dis_ed_pos_z','index_met_st_pos_x','index_met_st_pos_y','index_met_st_pos_z','index_met_ed_pos_x','index_met_ed_pos_y','index_met_ed_pos_z','index_pro_ed_pos_x','index_pro_ed_pos_y','index_pro_ed_pos_z','index_mid_ed_pos_x','index_mid_ed_pos_y','index_mid_ed_pos_z','index_dis_ed_pos_x','index_dis_ed_pos_y','index_dis_ed_pos_z','middle_met_st_pos_x','middle_met_st_pos_y','middle_met_st_pos_z','middle_met_ed_pos_x','middle_met_ed_pos_y','middle_met_ed_pos_z','middle_pro_ed_pos_x','middle_pro_ed_pos_y','middle_pro_ed_pos_z','middle_mid_ed_pos_x','middle_mid_ed_pos_y','middle_mid_ed_pos_z','middle_dis_ed_pos_x','middle_dis_ed_pos_y','middle_dis_ed_pos_z','ring_met_st_pos_x','ring_met_st_pos_y','ring_met_st_pos_z','ring_met_ed_pos_x','ring_met_ed_pos_y','ring_met_ed_pos_z','ring_pro_ed_pos_x','ring_pro_ed_pos_y','ring_pro_ed_pos_z','ring_mid_ed_pos_x','ring_mid_ed_pos_y','ring_mid_ed_pos_z','ring_dis_ed_pos_x','ring_dis_ed_pos_y','ring_dis_ed_pos_z','pinky_met_st_pos_x','pinky_met_st_pos_y','pinky_met_st_pos_z','pinky_met_ed_pos_x','pinky_met_ed_pos_y','pinky_met_ed_pos_z','pinky_pro_ed_pos_x','pinky_pro_ed_pos_y','pinky_pro_ed_pos_z','pinky_mid_ed_pos_x','pinky_mid_ed_pos_y','pinky_mid_ed_pos_z','pinky_dis_ed_pos_x','pinky_dis_ed_pos_y','pinky_dis_ed_pos_z']


'''
coo=[None for i in range(75)]
reader_point = csv.DictReader(file_coo)

name_doc=[row for row in reader_point]
print (name_doc[])

'''



def get_point(head):
	reader_point = csv.DictReader(file_coo)
	coo=[row[head] for row in reader_point]
	file_coo.seek(0)
	return coo
part_coo=[None for i in range(len(name))]


for i in range(len(name)):
	 part_coo_process=get_point(name[i])
	 part_coo[i]=part_coo_process[-2]
	 part_coo[i]=round(float(part_coo[i])/10,4)
	 print(part_coo[i])




#print (pointlist)
