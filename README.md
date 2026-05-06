## Copy sdkconfig.mesh_defaults vào thư mục root và thêm dòng này vào Cmake 
Cmake root add: list(APPEND SDKCONFIG_DEFAULTS "sdkconfig.mesh_defaults")<br>

## Thêm vùng nhớ ble_mesh vào partition table
ble_mesh,       data,   nvs,              ,     0x8000

## Cấu hình mạng mesh bằng net key, app key mặc định

1. Truy cập vào file **cfg_srv.c** theo đường dẫn "components/bt/esp_ble_mesh/core"
2. Code<br>

```c
====== CONFIG ======
#define CID_TEST        0x0211
#define VND_MODEL_ID    0x0001
#define APP_IDX         0x0000
#define NET_IDX         0x0000
#define NODE_ADDR       0x0005
#define TEST_BLE_MESH_MODEL_OP_3(b0, cid)    ((((b0) << 16) | 0xC00000) | (cid))

// ====== VENDOR OPCODE ======
#define OP_VND_TEST  TEST_BLE_MESH_MODEL_OP_3(0xE2, CID_TEST)

// ====== HANDLER ======
static void vnd_handler(struct bt_mesh_model *model,
                        struct bt_mesh_msg_ctx *ctx,
                        struct net_buf_simple *buf)
{
    printf("Recv vendor opcode, len=%d\n", buf->len);

    for (int i = 0; i < buf->len; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

// ====== OPCODE TABLE ======
static const struct bt_mesh_model_op vnd_op[] = {
    { OP_VND_TEST, 0, vnd_handler },
    { 0, 0, NULL },
};

// ====== VENDOR MODEL ======
static struct bt_mesh_model vnd_model = {
    .vnd = {
        .company = CID_TEST,
        .id = VND_MODEL_ID,
    },
    .keys = { [0 ... CONFIG_BLE_MESH_MODEL_KEY_COUNT-1] = 0xffff },
    .groups = {0},
    .op = vnd_op,
};

// ====== FAKE ELEMENT ======
static struct bt_mesh_elem fake_elem = {
    .addr = NODE_ADDR,
    .model_count = 1,
    .models = &vnd_model,
};

// ====== CONFIG SERVER MODEL (FAKE) ======
static struct bt_mesh_model cfg_model = {
    .id =  0x0000,
    .elem = &fake_elem,
};

// ====== FAKE APPKEY ADD ======
void rd_app_key_add(uint8_t appKey[16])
{
    uint8_t data[19];
    struct net_buf_simple buf;

    net_buf_simple_init_with_data(&buf, data, sizeof(data));
    buf.len = 0;

    key_idx_pack(&buf, NET_IDX, APP_IDX);
    net_buf_simple_add_mem(&buf, appKey, 16);

    struct bt_mesh_msg_ctx ctx = {
        .net_idx = NET_IDX,
        .app_idx = APP_IDX,
        .addr = 0x0002,        // SRC
        .recv_dst = NODE_ADDR, //
        .recv_op = OP_APP_KEY_ADD,
    };

    app_key_add(&cfg_model, &ctx, &buf);
}

// ====== FAKE MODEL BIND ======
void rd_mod_app_bind(void)
{
    uint8_t data[8];
    struct net_buf_simple buf;

    net_buf_simple_init_with_data(&buf, data, sizeof(data));
    buf.len = 0;

    net_buf_simple_add_le16(&buf, NODE_ADDR);
    net_buf_simple_add_le16(&buf, APP_IDX);
    net_buf_simple_add_le16(&buf, CID_TEST);
    net_buf_simple_add_le16(&buf, VND_MODEL_ID);

    struct bt_mesh_msg_ctx ctx = {
        .net_idx = NET_IDX,
        .app_idx = APP_IDX,
        .addr = 0x0002,
        .recv_dst = NODE_ADDR,
        .recv_op = OP_MOD_APP_BIND,
    };

    mod_app_bind(&cfg_model, &ctx, &buf);

    vnd_model.keys[0] = APP_IDX;
}
```