#ifndef _RD_BLE_MESH_H__
#define _RD_BLE_MESH_H__

#include "stdint.h"
#include "esp_err.h"
#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(EVENT_MESH_CONFIG_SERVER);
ESP_EVENT_DECLARE_BASE(EVENT_MESH_PROVISIONING);
ESP_EVENT_DECLARE_BASE(EVENT_MESH_GENERIC_MODEL);
ESP_EVENT_DECLARE_BASE(EVENT_MESH_LIGHTING_MODEL);

#define CONFIG_MAX_NUM_ELEMENT  3
#define CONFIG_USE_SDK_ROGO     1

typedef void *ble_mesh_cb_param_t; //ble_mesh_cb_param_t ~ (void *)
typedef void (*rd_handle_message_opcode_vender)(ble_mesh_cb_param_t param);

typedef enum {
    EVENT_MESH_ADD_APP_KEY = 0,
    EVENT_MESH_BIND_ALL,
    EVENT_MESH_SUB_ADD_GROUP,
    EVENT_MESH_SUB_DELETE_GROUP
} rd_mesh_cfg_server_event_t;

typedef enum {
    EVENT_MESH_PROVISION_COMPLETE = 0,
    EVENT_MESH_PROVISION_RESET
} rd_mesh_provisioning_event_t;

typedef enum {
    EVENT_CONTROLL_ONOFF_BY_UNICAST_ADDR = 0,
    EVENT_CONTROLL_ONOFF_BY_GROUP_ADDR,
    EVENT_CONTROLL_ONOFF_ALL
} rd_mesh_generic_model_event_t;

typedef enum {
    EVENT_CONTROLL_LIGHTNESS = 0,
    EVENT_CONTROLL_CTL_TEMPERATURE
} rd_mesh_lighting_model_event_t;

void      rd_ble_mesh_init(void);
void      rd_continue_ble_mesh(void);
void      rd_suspend_ble_mesh(void);

esp_err_t rd_ble_mesh_event_init(void);
esp_err_t rd_ble_mesh_register_event_handler(esp_event_base_t evt_base, esp_event_handler_t event_handler);
void      rd_ble_mesh_register_cb_handle_mess_opcode_E0(rd_handle_message_opcode_vender cb);
void      rd_ble_mesh_register_cb_handle_mess_opcode_E2(rd_handle_message_opcode_vender cb);

void      ble_mesh_get_mess_buf(ble_mesh_cb_param_t param, uint8_t **buff, uint16_t *len);
uint32_t  ble_mesh_get_opcode(ble_mesh_cb_param_t param);
esp_err_t ble_mesh_rsp_opcode_vender_E0(ble_mesh_cb_param_t param, uint8_t *par, uint8_t len);
esp_err_t ble_mesh_rsp_opcode_vender_E2(ble_mesh_cb_param_t param, uint8_t *par, uint8_t len);
esp_err_t ble_mesh_rsp_state(uint8_t eleIdx, uint8_t onoff);
void      ble_mesh_set_gw_addr(uint16_t addr);
uint16_t  ble_mesh_get_gw_addr(void);
void      ble_mesh_kick_out(void);

#endif /* _RD_BLE_MESH_H__ */
