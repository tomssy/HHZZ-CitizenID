import adsk.core, adsk.fusion, traceback

import csv
import os
# 从指定位置读取csv至字典（现在还很不优雅,更新后需要手动更换地址）
file_path=os.path.dirname(os.path.realpath(__file__))
file_path=file_path+'\handrecord.csv'
file_coo = open(file_path)
name=['thumb_met_st_pos_x','thumb_met_st_pos_y','thumb_met_st_pos_z','thumb_met_ed_pos_x','thumb_met_ed_pos_y','thumb_met_ed_pos_z','thumb_pro_ed_pos_x','thumb_pro_ed_pos_y','thumb_pro_ed_pos_z','thumb_mid_ed_pos_x','thumb_mid_ed_pos_y','thumb_mid_ed_pos_z','thumb_dis_ed_pos_x','thumb_dis_ed_pos_y','thumb_dis_ed_pos_z','index_met_st_pos_x','index_met_st_pos_y','index_met_st_pos_z','index_met_ed_pos_x','index_met_ed_pos_y','index_met_ed_pos_z','index_pro_ed_pos_x','index_pro_ed_pos_y','index_pro_ed_pos_z','index_mid_ed_pos_x','index_mid_ed_pos_y','index_mid_ed_pos_z','index_dis_ed_pos_x','index_dis_ed_pos_y','index_dis_ed_pos_z','middle_met_st_pos_x','middle_met_st_pos_y','middle_met_st_pos_z','middle_met_ed_pos_x','middle_met_ed_pos_y','middle_met_ed_pos_z','middle_pro_ed_pos_x','middle_pro_ed_pos_y','middle_pro_ed_pos_z','middle_mid_ed_pos_x','middle_mid_ed_pos_y','middle_mid_ed_pos_z','middle_dis_ed_pos_x','middle_dis_ed_pos_y','middle_dis_ed_pos_z','ring_met_st_pos_x','ring_met_st_pos_y','ring_met_st_pos_z','ring_met_ed_pos_x','ring_met_ed_pos_y','ring_met_ed_pos_z','ring_pro_ed_pos_x','ring_pro_ed_pos_y','ring_pro_ed_pos_z','ring_mid_ed_pos_x','ring_mid_ed_pos_y','ring_mid_ed_pos_z','ring_dis_ed_pos_x','ring_dis_ed_pos_y','ring_dis_ed_pos_z','pinky_met_st_pos_x','pinky_met_st_pos_y','pinky_met_st_pos_z','pinky_met_ed_pos_x','pinky_met_ed_pos_y','pinky_met_ed_pos_z','pinky_pro_ed_pos_x','pinky_pro_ed_pos_y','pinky_pro_ed_pos_z','pinky_mid_ed_pos_x','pinky_mid_ed_pos_y','pinky_mid_ed_pos_z','pinky_dis_ed_pos_x','pinky_dis_ed_pos_y','pinky_dis_ed_pos_z']


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
     #print(part_coo[i])

ui = None
try: 
    app = adsk.core.Application.get()
    ui = app.userInterface

    doc = app.documents.add(adsk.core.DocumentTypes.FusionDesignDocumentType)
    design = app.activeProduct

    # Get the root component of the active design.
    rootComp = design.rootComponent

    # Create a new sketch on the xy plane.
    sketches = rootComp.sketches;
    xyPlane = rootComp.xYConstructionPlane
    sketch = sketches.add(xyPlane)
    lines = sketch.sketchCurves.sketchLines
    # Draw two connected lines.
    

    num=0
    for finger in range(0,5): 
        points = adsk.core.ObjectCollection.create()
        for bone in range(0,5):

            points.add(adsk.core.Point3D.create(part_coo[num], 
                                                part_coo[num+1],
                                                part_coo[num+2]))
            num=num+3
        
        sketch.sketchCurves.sketchFittedSplines.add(points)

except:
    if ui:
        ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))

