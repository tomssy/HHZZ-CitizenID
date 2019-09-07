# -*- coding: utf-8 -*-
"""
Created on Sun Apr 22 16:00:24 2018

@author: jxw10
"""

import adsk.core, adsk.fusion, traceback


# 从指定位置读取csv至字典（现在还很不优雅,更新后需要手动更换地址）




def run(context):
    ui = None
    try:
        app = adsk.core.Application.get()
        ui  = app.userInterface
        des = adsk.fusion.Design.cast(app.activeProduct)

        paramName_x= 'x'
        paramName_y= 'y'
        paramName_z= 'z'

        param_x=des.allParameters.itemByName(paramName_x)
        param_y=des.allParameters.itemByName(paramName_y)
        param_z=des.allParameters.itemByName(paramName_z)
        #寻找参数
        param_x.value = 1
        param_y.value = 1
        param_z.value = 1
                    
    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))