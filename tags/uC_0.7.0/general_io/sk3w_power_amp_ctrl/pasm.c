/*****************************************************************************
* Model: pasm.qm
* File:  ./pasm.c
*
* This file has been generated automatically by QP Modeler (QM).
* DO NOT EDIT THIS FILE MANUALLY.
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
/* @(/1/0) .................................................................*/
/* @(/1/0/3) ...............................................................*/
/* @(/1/0/3/0) */
QState Pa_initial(Pa *me) {
    return Q_TRAN(&Pa_PowerOff);
}
/* @(/1/0/3/1) .............................................................*/
QState Pa_PowerOff(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/1) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_PowerOff/ENTRY\r\n");
            Pa_setOpStatus(me, AMP_OP_STATUS_OFF);
            bsp_set_pa_mains(me->band, 0);
            return Q_HANDLED();
        }
        /* @(/1/0/3/1) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_PowerOff/EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/1/0) */
        case TOGGLE_MAINS_SIG: {
            DEBUG_PRINT("Pa_PowerOff/TOGGLE_MAINS\r\n");
            return Q_TRAN(&Pa_PowerOn);
        }
        /* @(/1/0/3/1/1) */
        case BAND_SELECTED_SIG: {
            DEBUG_PRINT("Pa_powerOff/BAND_SELECTED\r\n");
            Pa_setCtrlr(me, Q_PAR(me));
            return Q_HANDLED();
        }
        /* @(/1/0/3/1/2) */
        case BAND_UNSELECTED_SIG: {
            DEBUG_PRINT("Pa_powerOff/BAND_UNSELECTED\r\n");
            Pa_setCtrlr(me, PA_CTRLR_UNUSED);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/3/2) .............................................................*/
QState Pa_PowerOn(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_PowerOn/ENTRY\r\n");
            bsp_set_pa_mains(me->band, 1);
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/0) */
        case Q_INIT_SIG: {
            DEBUG_PRINT("Pa_PowerOn/INIT\r\n");
            return Q_TRAN(&Pa_Warmup);
        }
        /* @(/1/0/3/2/1) */
        case BAND_SELECTED_SIG: {
            DEBUG_PRINT("Pa_powerOn/BAND_SELECTED\r\n");
            Pa_setCtrlr(me, Q_PAR(me));
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/2) */
        case BAND_UNSELECTED_SIG: {
            DEBUG_PRINT("Pa_powerOn/BAND_UNSELECTED\r\n");
            Pa_setCtrlr(me, PA_CTRLR_UNUSED);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/3/2/3) ...........................................................*/
QState Pa_Warmup(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2/3) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_Warmup/ENTRY\r\n");
            Pa_setOpStatus(me, AMP_OP_STATUS_WARMUP);
            QActive_arm((QActive *)me, cfg.warmup_timeout);
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/3) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_Warmup/EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/3/0) */
        case TOGGLE_MAINS_SIG: {
            DEBUG_PRINT("Pa_Warmup/TOGGLE_MAINS\r\n");
            return Q_TRAN(&Pa_PowerOff);
        }
        /* @(/1/0/3/2/3/1) */
        case Q_TIMEOUT_SIG: {
            DEBUG_PRINT("Pa_warmup/TIMEOUT\r\n");
            return Q_TRAN(&Pa_Operational);
        }
    }
    return Q_SUPER(&Pa_PowerOn);
}
/* @(/1/0/3/2/4) ...........................................................*/
QState Pa_Operational(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2/4) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_Operational/ENTRY\r\n");
            Pa_setOpStatus(me, AMP_OP_STATUS_READY);
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_Operational/EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/0) */
        case TOGGLE_MAINS_SIG: {
            DEBUG_PRINT("Pa_Operational/TOGGLE_MAINS\r\n");
            return Q_TRAN(&Pa_Cooldown);
        }
        /* @(/1/0/3/2/4/1) */
        case Q_INIT_SIG: {
            DEBUG_PRINT("Pa_Operational/INIT\r\n");
            /* @(/1/0/3/2/4/1/0) */
            if (me->ctrlr != PA_CTRLR_UNUSED) {
                return Q_TRAN(&Pa_Ready);
            }
            /* @(/1/0/3/2/4/1/1) */
            else {
                return Q_TRAN(&Pa_Unused);
            }
        }
        /* @(/1/0/3/2/4/2) */
        case Q_TIMEOUT_SIG: {
            DEBUG_PRINT("Pa_Operational/TIMEOUT\r\n");
            return Q_TRAN(&Pa_PowerOff);
        }
    }
    return Q_SUPER(&Pa_PowerOn);
}
/* @(/1/0/3/2/4/3) .........................................................*/
QState Pa_Transmitting(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2/4/3) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_Transmitting/ENTRY\r\n");
            bsp_set_pa_ptt(me->band, 1);
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/3) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_Transmitting/EXIT\r\n");
            bsp_set_pa_ptt(me->band, 0);
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/3/0) */
        case TX_ACTIVE_OFF_SIG: {
            DEBUG_PRINT("Pa_Transmitting/TX_ACTIVE_OFF\r\n");
            return Q_TRAN(&Pa_Ready);
        }
        /* @(/1/0/3/2/4/3/1) */
        case TOGGLE_MAINS_SIG: {
            DEBUG_PRINT("Pa_transmitting/TOGGLE_MAINS\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/3/2) */
        case BAND_UNSELECTED_SIG: {
            DEBUG_PRINT("Pa_Transmitting/BAND_UNSELECTED\r\n");
            return Q_TRAN(&Pa_Unused);
        }
    }
    return Q_SUPER(&Pa_Operational);
}
/* @(/1/0/3/2/4/4) .........................................................*/
QState Pa_Unused(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2/4/4) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_Unused/ENTRY\r\n");
            Pa_setCtrlr(me, PA_CTRLR_UNUSED);
            if (cfg.unused_timeout > 0) {
              QActive_arm((QActive *)me, cfg.unused_timeout);
            }
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/4) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_Unused/EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/4/0) */
        case BAND_SELECTED_SIG: {
            DEBUG_PRINT("Pa_unused/BAND_SELECTED\r\n");
            Pa_setCtrlr(me, Q_PAR(me));
            return Q_TRAN(&Pa_Ready);
        }
    }
    return Q_SUPER(&Pa_Operational);
}
/* @(/1/0/3/2/4/5) .........................................................*/
QState Pa_Ready(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2/4/5) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_Ready/ENTRY\r\n");
            if (cfg.unused_timeout > 0) {
              QActive_arm((QActive *)me, cfg.unused_timeout);
            }
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/5) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_Ready/EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/4/5/0) */
        case BAND_UNSELECTED_SIG: {
            DEBUG_PRINT("Pa_Ready/BAND_UNSELECTED\r\n");
            return Q_TRAN(&Pa_Unused);
        }
        /* @(/1/0/3/2/4/5/1) */
        case TX_ACTIVE_ON_SIG: {
            DEBUG_PRINT("Pa_Ready/TX_ACTIVE_ON\r\n");
            return Q_TRAN(&Pa_Transmitting);
        }
    }
    return Q_SUPER(&Pa_Operational);
}
/* @(/1/0/3/2/5) ...........................................................*/
QState Pa_Cooldown(Pa *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/3/2/5) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Pa_Cooldown/ENTRY\r\n");
            Pa_setOpStatus(me, AMP_OP_STATUS_COOLDOWN);
            QActive_arm((QActive *)me, cfg.cooldown_timeout);
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/5) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Pa_cooldown/EXIT\r\n");
            return Q_HANDLED();
        }
        /* @(/1/0/3/2/5/0) */
        case Q_TIMEOUT_SIG: {
            DEBUG_PRINT("Pa_Cooldown/TIMEOUT\r\n");
            return Q_TRAN(&Pa_PowerOff);
        }
        /* @(/1/0/3/2/5/1) */
        case TOGGLE_MAINS_SIG: {
            DEBUG_PRINT("Pa_Cooldown/TOGGLE_MAINS\r\n");
            return Q_TRAN(&Pa_Operational);
        }
    }
    return Q_SUPER(&Pa_PowerOn);
}

