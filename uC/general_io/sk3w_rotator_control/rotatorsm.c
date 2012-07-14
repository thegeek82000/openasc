/*****************************************************************************
* Model: rotatorsm.qm
* File:  ./rotatorsm.c
*
* This file has been generated automatically by QP Modeler (QM).
* DO NOT EDIT THIS FILE MANUALLY.
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
/* @(/1/0) .................................................................*/
/* @(/1/0/11) ..............................................................*/
/* @(/1/0/11/0) */
QState Rotator_initial(Rotator *me) {
    return Q_TRAN(&Rotator_Idle);
}
/* @(/1/0/11/1) ............................................................*/
QState Rotator_Idle(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/1) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Idle: ENTER\r\n");
            me->rotate_dir = 0;
            me->target_heading = INT16_MAX;
            return Q_HANDLED();
        }
        /* @(/1/0/11/1) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Idle: EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/11/1/0) */
        case CAL_ENABLE_SIG: {
            return Q_TRAN(&Rotator_Calibrate);
        }
        /* @(/1/0/11/1/1) */
        case ROTATE_CW_SIG: {
            /* @(/1/0/11/1/1/1) */
            if (cw_limit_exceeded(me)) {
                return Q_TRAN(&Rotator_Idle);
            }
            /* @(/1/0/11/1/1/0) */
            else {
                me->rotate_dir = 1;
                return Q_TRAN(&Rotator_BreakReleased);
            }
        }
        /* @(/1/0/11/1/2) */
        case ROTATE_CCW_SIG: {
            /* @(/1/0/11/1/2/1) */
            if (ccw_limit_exceeded(me)) {
                return Q_TRAN(&Rotator_Idle);
            }
            /* @(/1/0/11/1/2/0) */
            else {
                me->rotate_dir = -1;
                return Q_TRAN(&Rotator_BreakReleased);
            }
        }
        /* @(/1/0/11/1/3) */
        case ROTATOR_ERROR_SIG: {
            return Q_TRAN(&Rotator_Error);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/11/2) ............................................................*/
QState Rotator_Calibrate(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/2) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Calibrate: ENTER\r\n");
            bsp_rotator_release_break(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/2) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Calibrate: EXIT\r\n");
            bsp_rotator_stop(me->rot_idx);
            bsp_rotator_apply_break(me->rot_idx);
            me->error = 0;
            Rotator_calc_heading_coeffs(me);
            eeprom_write_config();
            return Q_HANDLED();
        }
        /* @(/1/0/11/2/0) */
        case CAL_DISABLE_SIG: {
            return Q_TRAN(&Rotator_Idle);
        }
        /* @(/1/0/11/2/1) */
        case ROTATE_CW_SIG: {
            bsp_rotator_run_cw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/2/2) */
        case ROTATE_CCW_SIG: {
            bsp_rotator_run_ccw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/2/3) */
        case STOP_SIG: {
            bsp_rotator_stop(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/2/4) */
        case SET_CCW_LIMIT_SIG: {
            Rotator_set_ccw_limit(me, Q_PAR(me));
            return Q_HANDLED();
        }
        /* @(/1/0/11/2/5) */
        case SET_CW_LIMIT_SIG: {
            Rotator_set_cw_limit(me, Q_PAR(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/11/3) ............................................................*/
QState Rotator_BreakReleased(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/3) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("BreakReleased: ENTER\r\n");
            bsp_rotator_release_break(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("BreakReleased: EXIT\r\n");
            me->rotate_dir = 0;
            me->target_heading = INT16_MAX;
            bsp_rotator_apply_break(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/0) */
        case Q_INIT_SIG: {
            return Q_TRAN(&Rotator_StartWait);
        }
        /* @(/1/0/11/3/1) */
        case ROTATE_CW_SIG: {
            Rotator_set_rotate_dir(me, 1);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/2) */
        case ROTATE_CCW_SIG: {
            Rotator_set_rotate_dir(me, -1);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/3) */
        case ROTATOR_ERROR_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_TRAN(&Rotator_StopWait);
        }
        /* @(/1/0/11/3/4) */
        case STOP_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/11/3/5) ..........................................................*/
QState Rotator_StartWait(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/3/5) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("StartWait: ENTER\r\n");
            QActive_arm((QActive *)me, 10);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/5) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("StartWait: EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/5/0) */
        case Q_TIMEOUT_SIG: {
            return Q_TRAN(&Rotator_Running);
        }
        /* @(/1/0/11/3/5/1) */
        case STOP_SIG: {
            return Q_TRAN(&Rotator_Idle);
        }
    }
    return Q_SUPER(&Rotator_BreakReleased);
}
/* @(/1/0/11/3/6) ..........................................................*/
QState Rotator_StopWait(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/3/6) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("StopWait: ENTER\r\n");
            QActive_arm((QActive *)me, 30);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/6) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("StopWait: EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/6/0) */
        case Q_TIMEOUT_SIG: {
            /* @(/1/0/11/3/6/0/0) */
            if (me->error != 0) {
                return Q_TRAN(&Rotator_Error);
            }
            /* @(/1/0/11/3/6/0/1) */
            else {
                /* @(/1/0/11/3/6/0/1/1) */
                if (me->rotate_dir == 0) {
                    return Q_TRAN(&Rotator_Idle);
                }
                /* @(/1/0/11/3/6/0/1/0) */
                else {
                    return Q_TRAN(&Rotator_Running);
                }
            }
        }
    }
    return Q_SUPER(&Rotator_BreakReleased);
}
/* @(/1/0/11/3/7) ..........................................................*/
QState Rotator_Running(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/3/7) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Running: ENTER\r\n");
            Q_REQUIRE(me->rotate_dir != 0);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Running: EXIT\r\n");
            me->target_heading = INT16_MAX;
            bsp_rotator_stop(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/0) */
        case Q_INIT_SIG: {
            /* @(/1/0/11/3/7/0/1) */
            if (me->rotate_dir > 0) {
                return Q_TRAN(&Rotator_CW);
            }
            /* @(/1/0/11/3/7/0/0) */
            else {
                return Q_TRAN(&Rotator_CCW);
            }
        }
        /* @(/1/0/11/3/7/1) */
        case STOP_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_TRAN(&Rotator_StopWait);
        }
        /* @(/1/0/11/3/7/2) */
        case ROTATION_LIMIT_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_TRAN(&Rotator_StopWait);
        }
    }
    return Q_SUPER(&Rotator_BreakReleased);
}
/* @(/1/0/11/3/7/3) ........................................................*/
QState Rotator_CCW(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/3/7/3) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("CCW: ENTER\r\n");
            bsp_rotator_run_ccw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/3) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("CCW: EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/3/0) */
        case ROTATE_CCW_SIG: {
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/3/1) */
        case ROTATE_CW_SIG: {
            Rotator_set_rotate_dir(me, 1);
            return Q_TRAN(&Rotator_StopWait);
        }
    }
    return Q_SUPER(&Rotator_Running);
}
/* @(/1/0/11/3/7/4) ........................................................*/
QState Rotator_CW(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/3/7/4) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("CW: ENTER\r\n");
            bsp_rotator_run_cw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/4) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("CW: EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/4/0) */
        case ROTATE_CW_SIG: {
            return Q_HANDLED();
        }
        /* @(/1/0/11/3/7/4/1) */
        case ROTATE_CCW_SIG: {
            Rotator_set_rotate_dir(me, -1);
            return Q_TRAN(&Rotator_StopWait);
        }
    }
    return Q_SUPER(&Rotator_Running);
}
/* @(/1/0/11/4) ............................................................*/
QState Rotator_Error(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/11/4) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Error: ENTER\r\n");
            me->rotate_dir = 0;
            send_ascii_data(0, "ROTATOR #%d ERROR: %s\r\n", me->rot_idx, Rotator_strerror(me));
            return Q_HANDLED();
        }
        /* @(/1/0/11/4) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Error: EXIT\r\n");
            me->rotate_dir = 0;
            me->error = 0;
            send_ascii_data(0, "ROTATOR #%d ERROR CLEARED\r\n", me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/11/4/0) */
        case ROTATE_CW_SIG: {
            if (++me->rotate_dir == 2) {
              QActive_arm((QActive *)me, 30);
            }
            return Q_HANDLED();
        }
        /* @(/1/0/11/4/1) */
        case ROTATE_CCW_SIG: {
            if (++me->rotate_dir == 2) {
              QActive_arm((QActive *)me, 30);
            }
            return Q_HANDLED();
        }
        /* @(/1/0/11/4/2) */
        case Q_TIMEOUT_SIG: {
            return Q_TRAN(&Rotator_Idle);
        }
        /* @(/1/0/11/4/3) */
        case STOP_SIG: {
            me->rotate_dir = 0;
            QActive_disarm((QActive *)me);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

