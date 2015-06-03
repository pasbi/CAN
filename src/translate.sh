#!/bin/bash
lupdate CAN2.pro
lupdate CAN2.pro
linguist can2_*.ts
lrelease CAN2.pro
mv *.qm ../build-CAN2-Desktop-Debug/
