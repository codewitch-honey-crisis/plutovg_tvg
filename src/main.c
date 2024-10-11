#include <math.h>
#include <plutovg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
enum {
    // end of document This command determines the end of file.
    TVG_CMD_END_DOCUMENT = 0,
    // fill polygon This command fills an N-gon.
    TVG_CMD_FILL_POLYGON,
    // fill rectangles This command fills a set of rectangles.
    TVG_CMD_FILL_RECTANGLES,
    // fill path This command fills a free-form path.
    TVG_CMD_FILL_PATH,
    // draw lines This command draws a set of lines.
    TVG_CMD_DRAW_LINES,
    // draw line loop This command draws the outline of a polygon.
    TVG_CMD_DRAW_LINE_LOOP,
    // draw line strip This command draws a list of end-to-end lines.
    TVG_CMD_DRAW_LINE_STRIP,
    // draw line path This command draws a free-form path.
    TVG_CMD_DRAW_LINE_PATH,
    // outline fill polygon This command draws a filled polygon with an outline.
    TVG_CMD_OUTLINE_FILL_POLYGON,
    // outline fill rectangles This command draws several filled rectangles with
    // an outline.
    TVG_CMD_OUTLINE_FILL_RECTANGLES,
    // outline fill path This command combines the fill and draw line path
    // command into one
    TVG_CMD_OUTLINE_FILL_PATH
};
enum {
    // solid color
    TVG_STYLE_FLAT = 0,
    // linear gradient
    TVG_STYLE_LINEAR,
    // radial gradient
    TVG_STYLE_RADIAL
};
enum {
    // unit uses 16 bit,
    TVG_RANGE_DEFAULT = 0,
    // unit takes only 8 bit
    TVG_RANGE_REDUCED,
    // unit uses 32 bit,
    TVG_RANGE_ENHANCED,
};

/// The color encoding used in a TinyVG file.
// This enum describes how the data in the color table
// section of the format looks like.
enum {
    // A classic 4-tuple with 8 bit unsigned channels.
    // Encodes red, green, blue and alpha. If not specified
    // otherwise (via external means) the color channels encode
    // sRGB color data and the alpha stores linear transparency.
    TVG_COLOR_U8888 = 0,
    // A 16 bit color format with 5 bit for red and blue, and
    // 6 bit color depth for green channel.
    // This format is typically used in embedded devices or cheaper
    // displays. If not specified otherwise (via external means) the
    // color channels encode sRGB color data.
    TVG_COLOR_U565,
    // A format with 16 byte per color and 4 channels. Each channel
    // is encoded as a `binary32` IEEE 754 value.
    // The first three channels encode color data, the fourth
    // channel encodes linear alpha.
    // If not specified otherwise (via external means) the color
    // channels encode sRGB color data and the alpha stores linear
    // transparency.
    TVG_COLOR_F32,
    // This format is specified by external means and is meant to
    // signal that these files are *valid*, but it's not possible
    // to decode them without external knowledge about the color
    // encoding. This is meant for special cases where huge savings
    // might be possible by not encoding any color information in
    // the files itself or special device dependent color formats
    // are required.
    //
    // Possible uses cases are:
    //
    // - External fixed or shared color palettes
    // - CMYK format for printing
    // - High precision 16 bit color formats
    // - Using non-sRGB color spaces
    // - Using RAL numbers for painting
    // - ...
    //
    // **NOTE:** A conforming parser is allowed to reject any file with a
    // custom color encoding, as these are meant to be parsed with a
    // specific use case.
    TVG_COLOR_CUSTOM,
};

// A TinyVG scale value. Defines the scale for all units inside a graphic.
// The scale is defined by the number of decimal bits in a `i32`, thus scaling
// can be trivially implemented by shifting the integers right by the scale
// bits.
enum {
    TVG_SCALE_1_1 = 0,
    TVG_SCALE_1_2,
    TVG_SCALE_1_4,
    TVG_SCALE_1_8,
    TVG_SCALE_1_16,
    TVG_SCALE_1_32,
    TVG_SCALE_1_64,
    TVG_SCALE_1_128,
    TVG_SCALE_1_256,
    TVG_SCALE_1_512,
    TVG_SCALE_1_1024,
    TVG_SCALE_1_2048,
    TVG_SCALE_1_4096,
    TVG_SCALE_1_8192,
    TVG_SCALE_1_16384,
    TVG_SCALE_1_32768,
};
// path commands
enum {
    TVG_PATH_LINE = 0,
    TVG_PATH_HLINE,
    TVG_PATH_VLINE,
    TVG_PATH_CUBIC,
    TVG_PATH_ARC_CIRCLE,
    TVG_PATH_ARC_ELLIPSE,
    TVG_PATH_CLOSE,
    TVG_PATH_QUAD
};
// error codes
enum {
    TVG_SUCCESS = 0,
    TVG_E_INVALID_ARG,
    TVG_E_INVALID_STATE,
    TVG_E_INVALID_FORMAT,
    TVG_E_IO_ERROR,
    TVG_E_OUT_OF_MEMORY,
    TVG_E_NOT_SUPPORTED
};

#define TVG_PI (3.1415926536f)
// clamp a value to a range
#define TVG_CLAMP(x, mn, mx) (x > mx ? mx : (x < mn ? mn : x))
// get the red channel of an RGB565 color
#define TVG_RGB16_R(x) (x & 0x1F)
// get the green channel of an RGB565 color
#define TVG_RGB16_G(x) ((x >> 5) & 0x3F)
// get the blue channel of an RGB565 color
#define TVG_RGB16_B(x) ((x >> 11) & 0x1F)
// get the index of the command
// essentially the command id
#define TVG_CMD_INDEX(x) (x & 0x3F)
// get the style kind flags in the command
#define TVG_CMD_STYLE_KIND(x) ((x >> 6) & 0x3)
// get the packed size out of the size
// and style kind packed value
#define TVG_SIZE_AND_STYLE_SIZE(x) ((x & 0x3F) + 1)
// get the style kind out of the size
// and style kind packed value
#define TVG_SIZE_AND_STYLE_STYLE_KIND(x) ((x >> 6) & 0x3)
// get the scale from the header
#define TVG_HEADER_DATA_SCALE(x) (x & 0x0F)
// get the color encoding from the header
#define TVG_HEADER_DATA_COLOR_ENC(x) ((x >> 4) & 0x03)
// get the color range from the header
#define TVG_HEADER_DATA_RANGE(x) ((x >> 6) & 0x03)
// get the path command index/id
#define TVG_PATH_CMD_INDEX(x) (x & 0x7)
// flag indicating the path has
// line/stroke to it
#define TVG_PATH_CMD_HAS_LINE(x) ((x >> 4) & 0x1)
// flag indicating the arc is a large arc
#define TVG_ARC_LARGE(x) (x & 0x1)
// flag indicating the sweep direction
// 0=left, 1=right
#define TVG_ARC_SWEEP(x) ((x >> 1) & 1)

// F32 pixel format
typedef struct {
    float r, g, b, a;
} tvg_f32_pixel_t;

// rgba32 color struct
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} tvg_rgba32_t;

// TVG internal color struct
typedef struct {
    float r;
    float g;
    float b;
} tvg_rgb_t;

// TVG internal color struct
// with alpha channel
typedef struct {
    float r;
    float g;
    float b;
    float a;
} tvg_rgba_t;

// coordinate
typedef struct {
    float x;
    float y;
} tvg_point_t;

// rectangle
typedef struct {
    float x;
    float y;
    float width;
    float height;
} tvg_rect_t;

// gradient data
typedef struct {
    tvg_point_t point0;
    tvg_point_t point1;
    uint32_t color0;
    uint32_t color1;
} tvg_gradient_t;

// style data
typedef struct {
    uint8_t kind;
    uint32_t flat;  // color index
    union { 
        tvg_gradient_t linear;
        tvg_gradient_t radial;
    };
} tvg_style_t;

// fill header
typedef struct {
    tvg_style_t style;
    size_t size;
} tvg_fill_header_t;

// line header
typedef struct {
    tvg_style_t style;
    float line_width;
    size_t size;
} tvg_line_header_t;

// outline and fill header
typedef struct {
    tvg_style_t fill_style;
    tvg_style_t line_style;
    float line_width;
    size_t size;
} tvg_outline_fill_header_t;

typedef size_t (*tvg_input_func_t)(uint8_t* data, size_t size, void* state);

typedef struct {
    tvg_input_func_t inp;
    void* inp_state;
    plutovg_canvas_t* cvs;
    uint8_t scale;
    uint8_t color_encoding;
    uint8_t coord_range;
    uint32_t width, height;
    size_t colors_size;
    tvg_rgba_t* colors;
} tvg_context_t;

typedef int result_t;

static uint32_t tvg_map_zero_to_max(tvg_context_t* ctx, uint32_t value) {
    if (0 == value) {
        switch (ctx->coord_range) {
            case TVG_RANGE_DEFAULT:
                return 0xFFFF;
            case TVG_RANGE_REDUCED:
                return 0xFF;
            default:
                return 0xFFFFFFFF;
        }
    }
    return value;
}

static result_t tvg_read_coord(tvg_context_t* ctx, uint32_t* out_raw_value) {
    size_t read;
    switch (ctx->coord_range) {
        case TVG_RANGE_DEFAULT: {
            uint16_t u16;
            read = ctx->inp((uint8_t*)&u16, sizeof(uint16_t), ctx->inp_state);
            if (sizeof(uint16_t) > read) {
                return TVG_E_IO_ERROR;
            }
            *out_raw_value = u16;
            return TVG_SUCCESS;
        }
        case TVG_RANGE_REDUCED: {
            uint8_t u8;
            read = ctx->inp((uint8_t*)&u8, sizeof(uint8_t), ctx->inp_state);
            if (sizeof(uint8_t) > read) {
                return TVG_E_IO_ERROR;
            }
            *out_raw_value = u8;
            return TVG_SUCCESS;
        }
        default:
            read = ctx->inp((uint8_t*)out_raw_value, sizeof(uint32_t),
                            ctx->inp_state);
            if (sizeof(uint32_t) > read) {
                return TVG_E_IO_ERROR;
            }
            return TVG_SUCCESS;
    }
}

static result_t tvg_read_color(tvg_context_t* ctx, tvg_rgba_t* out_color) {
    size_t read;
    switch (ctx->color_encoding) {
        case TVG_COLOR_F32: {
            tvg_f32_pixel_t data;
            read = ctx->inp((uint8_t*)&data, sizeof(data), ctx->inp_state);
            if (sizeof(data) > read) {
                return TVG_E_IO_ERROR;
            }
            out_color->r = data.r;
            out_color->g = data.g;
            out_color->b = data.b;
            out_color->a = data.a;
            return TVG_SUCCESS;
        }
        case TVG_COLOR_U565: {
            uint16_t data;
            read = ctx->inp((uint8_t*)&data, sizeof(data), ctx->inp_state);
            if (sizeof(data) > read) {
                return TVG_E_IO_ERROR;
            }
            out_color->r = ((float)TVG_RGB16_R(data)) / 15.0f;
            out_color->g = ((float)TVG_RGB16_G(data)) / 31.0f;
            out_color->b = ((float)TVG_RGB16_B(data)) / 15.0f;
            out_color->a = 1.0f;
            return TVG_SUCCESS;
        }
        case TVG_COLOR_U8888: {
            tvg_rgba32_t data;
            read = ctx->inp((uint8_t*)&data.r, 1, ctx->inp_state);
            if (1 > read) {
                return TVG_E_IO_ERROR;
            }
            read = ctx->inp((uint8_t*)&data.g, 1, ctx->inp_state);
            if (1 > read) {
                return TVG_E_IO_ERROR;
            }
            read = ctx->inp((uint8_t*)&data.b, 1, ctx->inp_state);
            if (1 > read) {
                return TVG_E_IO_ERROR;
            }
            read = ctx->inp((uint8_t*)&data.a, 1, ctx->inp_state);
            if (1 > read) {
                return TVG_E_IO_ERROR;
            }
            out_color->r = ((float)data.r) / 255.0f;
            out_color->g = ((float)data.g) / 255.0f;
            out_color->b = ((float)data.b) / 255.0f;
            out_color->a = ((float)data.a) / 255.0f;
            return TVG_SUCCESS;
        }
        case TVG_COLOR_CUSTOM:
            return TVG_E_NOT_SUPPORTED;
        default:
            return TVG_E_INVALID_FORMAT;
    }
}

static float tvg_downscale_coord(tvg_context_t* ctx, uint32_t coord) {
    uint16_t factor = (((uint16_t)1) << ctx->scale);
    return (float)coord / (float)factor;
}

static uint32_t tvg_upscale_coord(tvg_context_t* ctx, uint32_t coord) {
    uint16_t factor = (((uint16_t)1) << ctx->scale);
    return (uint32_t)((float)coord * (float)factor) + 0.5f;
    ;
}

static result_t tvg_read_u32(tvg_context_t* ctx, uint32_t* out_value) {
    int count = 0;
    uint32_t result = 0;
    uint8_t byte;
    while (true) {
        if (1 > ctx->inp(&byte, 1, ctx->inp_state)) {
            return TVG_E_IO_ERROR;
        }
        const uint32_t val = ((uint32_t)(byte & 0x7F)) << (7 * count);
        result |= val;
        if ((byte & 0x80) == 0) break;
        ++count;
    }
    *out_value = result;
    return TVG_SUCCESS;
}

static result_t tvg_read_unit(tvg_context_t* ctx, float* out_value) {
    uint32_t val;
    result_t res = tvg_read_coord(ctx, &val);
    if (res != TVG_SUCCESS) {
        return res;
    }
    *out_value = tvg_downscale_coord(ctx, val);
    return TVG_SUCCESS;
}

static result_t tvg_read_point(tvg_context_t* ctx, tvg_point_t* out_point) {
    float f32;
    result_t res = tvg_read_unit(ctx, &f32);
    if (res != TVG_SUCCESS) {
        return res;
    }
    out_point->x = f32;
    res = tvg_read_unit(ctx, &f32);
    if (res != TVG_SUCCESS) {
        return res;
    }
    out_point->y = f32;
    return TVG_SUCCESS;
}

static result_t tvg_parse_header(tvg_context_t* ctx, int dim_only) {
    uint8_t data[4];

    if (2 > ctx->inp((uint8_t*)data, 2, ctx->inp_state)) {
        return TVG_E_IO_ERROR;
    }
    if (data[0] != 0x72 || data[1] != 0x56) {
        return TVG_E_INVALID_FORMAT;
    }
    if (1 > ctx->inp(data, 1, ctx->inp_state)) {
        return TVG_E_IO_ERROR;
    }
    if (data[0] != 1) {
        return TVG_E_NOT_SUPPORTED;
    }
    if (1 > ctx->inp(data, 1, ctx->inp_state)) {
        return TVG_E_IO_ERROR;
    }
    ctx->scale = TVG_HEADER_DATA_SCALE(data[0]);
    ctx->color_encoding = TVG_HEADER_DATA_COLOR_ENC(data[0]);
    ctx->coord_range = TVG_HEADER_DATA_RANGE(data[0]);
    uint32_t tmp;
    result_t res = tvg_read_coord(ctx, &tmp);
    if (res != TVG_SUCCESS) {
        return res;
    }
    ctx->width = tvg_map_zero_to_max(ctx, tmp);
    res = tvg_read_coord(ctx, &tmp);
    if (res != TVG_SUCCESS) {
        return res;
    }
    ctx->height = tvg_map_zero_to_max(ctx, tmp);
    if (dim_only) {
        return TVG_SUCCESS;
    }
    uint32_t color_count;
    res = tvg_read_u32(ctx, &color_count);
    if (res != TVG_SUCCESS) {
        return res;
    }
    if (color_count == 0) {
        return TVG_E_INVALID_FORMAT;
    }
    ctx->colors = (tvg_rgba_t*)malloc(color_count * sizeof(tvg_rgba_t));
    if (ctx->colors == NULL) {
        return TVG_E_OUT_OF_MEMORY;
    }
    ctx->colors_size = (size_t)color_count;
    for (size_t i = 0; i < ctx->colors_size; ++i) {
        res = tvg_read_color(ctx, &ctx->colors[i]);
        if (res != TVG_SUCCESS) {
            free(ctx->colors);
            ctx->colors = NULL;
            return res;
        }
    }
    return TVG_SUCCESS;
}

static result_t tvg_parse_gradient(tvg_context_t* ctx,
                                   tvg_gradient_t* out_gradient) {
    uint32_t u32;
    tvg_point_t pt;
    result_t res = tvg_read_point(ctx, &pt);
    if (res != TVG_SUCCESS) {
        return res;
    }
    out_gradient->point0 = pt;
    res = tvg_read_point(ctx, &pt);
    if (res != TVG_SUCCESS) {
        return res;
    }
    out_gradient->point1 = pt;
    res = tvg_read_u32(ctx, &u32);
    if (res != TVG_SUCCESS) {
        return res;
    }
    out_gradient->color0 = u32;
    if (u32 > ctx->colors_size) {
        return TVG_E_INVALID_FORMAT;
    }
    res = tvg_read_u32(ctx, &u32);
    if (res != TVG_SUCCESS) {
        return res;
    }
    if (u32 > ctx->colors_size) {
        return TVG_E_INVALID_FORMAT;
    }
    out_gradient->color1 = u32;
    return TVG_SUCCESS;
}

static result_t tvg_parse_style(tvg_context_t* ctx, int kind,
                                tvg_style_t* out_style) {
    result_t res;
    uint32_t flat;
    tvg_gradient_t grad;
    out_style->kind = kind;
    switch (kind) {
        case TVG_STYLE_FLAT:
            res = tvg_read_u32(ctx, &flat);
            if (res != TVG_SUCCESS) {
                return res;
            }
            out_style->flat = flat;
            break;
        case TVG_STYLE_LINEAR:
            res = tvg_parse_gradient(ctx, &grad);
            out_style->linear = grad;
            break;
        case TVG_STYLE_RADIAL:
            res = tvg_parse_gradient(ctx, &grad);
            out_style->radial = grad;
            break;
        default:
            res = TVG_E_INVALID_FORMAT;
            break;
    }
    if (res != TVG_SUCCESS) {
        return res;
    }
    return res;
}

static result_t tvg_parse_fill_header(tvg_context_t* ctx, int kind,
                                      tvg_fill_header_t* out_header) {
    uint32_t u32;
    result_t res = tvg_read_u32(ctx, &u32);
    if (res != TVG_SUCCESS) {
        return res;
    }
    size_t count = (size_t)u32 + 1;
    out_header->size = count;
    res = tvg_parse_style(ctx, kind, &out_header->style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    return TVG_SUCCESS;
}

static float tvg_distance(const tvg_point_t* lhs, const tvg_point_t* rhs) {
    float xd = rhs->x - lhs->x;
    float yd = rhs->y - lhs->y;
    return sqrtf((xd * xd) + (yd * yd));
}

static plutovg_color_t tvg_color_to_plutovg(const tvg_rgba_t* col) {
    plutovg_color_t result;
    plutovg_color_init_rgba(&result, col->r, col->g, col->b, col->a);
    return result;
}

static result_t tvg_apply_style(tvg_context_t* ctx, const tvg_style_t* style) {
    plutovg_color_t col;
    float r;
    plutovg_gradient_stop_t stops[2];
    switch (style->kind) {
        case TVG_STYLE_FLAT:
            col = tvg_color_to_plutovg(&ctx->colors[style->flat]);
            plutovg_canvas_set_color(ctx->cvs, &col);
            break;
        case TVG_STYLE_LINEAR:
            col = tvg_color_to_plutovg(&ctx->colors[style->linear.color0]);
            stops[0].color = col;
            stops[0].offset = 0;
            col = tvg_color_to_plutovg(&ctx->colors[style->linear.color1]);
            stops[1].color = col;
            stops[1].offset = 1;
            plutovg_canvas_set_linear_gradient(
                ctx->cvs, style->linear.point0.x, style->linear.point0.y,
                style->linear.point1.x, style->linear.point1.y,
                PLUTOVG_SPREAD_METHOD_PAD, stops, 2, NULL);

            break;
        case TVG_STYLE_RADIAL:
            col = tvg_color_to_plutovg(&ctx->colors[style->radial.color0]);
            stops[0].color = col;
            stops[0].offset = 0;
            col = tvg_color_to_plutovg(&ctx->colors[style->radial.color1]);
            stops[1].color = col;
            stops[1].offset = 1;
            r = tvg_distance(&style->radial.point0, &style->radial.point1);
            plutovg_canvas_set_radial_gradient(
                ctx->cvs, style->radial.point0.x, style->radial.point0.y, r,
                style->radial.point1.x, style->radial.point1.y, r,
                PLUTOVG_SPREAD_METHOD_REFLECT, stops, 2, NULL);
            break;
        default:
            return TVG_E_INVALID_FORMAT;
    }
    return TVG_SUCCESS;
}

static result_t tvg_parse_fill_rectangles(tvg_context_t* ctx, size_t size,
                                          const tvg_style_t* fill_style) {
    size_t count = size;
    size_t szb = count * sizeof(tvg_rect_t);
    result_t res;
    tvg_point_t pt;
    plutovg_canvas_set_fill_rule(ctx->cvs, PLUTOVG_FILL_RULE_EVEN_ODD);
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    res = tvg_apply_style(ctx, fill_style);
    if (res != TVG_SUCCESS) return res;
    while (count--) {
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) return res;
        float w, h;
        res = tvg_read_unit(ctx, &w);
        if (res != TVG_SUCCESS) return res;
        res = tvg_read_unit(ctx, &h);
        if (res != TVG_SUCCESS) return res;
        plutovg_canvas_rect(ctx->cvs, pt.x, pt.y, w, h);
        plutovg_canvas_fill(ctx->cvs);
    }
    return TVG_SUCCESS;
}

static result_t tvg_parse_fill_polygon(tvg_context_t* ctx, size_t size,
                                       const tvg_style_t* fill_style) {
    size_t count = size;
    tvg_point_t pt;
    result_t res = tvg_read_point(ctx, &pt);
    if (res != TVG_SUCCESS) return res;
    plutovg_canvas_set_fill_rule(ctx->cvs, PLUTOVG_FILL_RULE_EVEN_ODD);
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    res = tvg_apply_style(ctx, fill_style);
    if (res != TVG_SUCCESS) return res;
    plutovg_canvas_move_to(ctx->cvs, pt.x, pt.y);
    while (--count) {
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) return res;
        plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
    }
    tvg_apply_style(ctx, fill_style);
    plutovg_canvas_fill(ctx->cvs);
    return TVG_SUCCESS;
}

static result_t tvg_parse_line_header(tvg_context_t* ctx, int kind,
                                      tvg_line_header_t* out_header) {
    uint32_t u32;
    result_t res = tvg_read_u32(ctx, &u32);
    if (res != TVG_SUCCESS) {
        return res;
    }
    size_t count = (size_t)u32 + 1;
    out_header->size = count;
    res = tvg_parse_style(ctx, kind, &out_header->style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    res = tvg_read_unit(ctx, &out_header->line_width);
    if (res != TVG_SUCCESS) {
        return res;
    }

    return TVG_SUCCESS;
}

static result_t tvg_parse_outline_fill_header(
    tvg_context_t* ctx, int kind, tvg_outline_fill_header_t* out_header) {
    uint32_t u32;
    uint8_t d;
    if (1 > ctx->inp(&d, 1, ctx->inp_state)) {
        return TVG_E_IO_ERROR;
    }
    result_t res = TVG_SUCCESS;

    size_t count = TVG_SIZE_AND_STYLE_SIZE(d);
    out_header->size = count;
    res = tvg_parse_style(ctx, kind, &out_header->fill_style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    res = tvg_parse_style(ctx, TVG_SIZE_AND_STYLE_STYLE_KIND(d),
                          &out_header->line_style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    res = tvg_read_unit(ctx, &out_header->line_width);
    if (res != TVG_SUCCESS) {
        return res;
    }

    return TVG_SUCCESS;
}

static result_t tvg_parse_path(tvg_context_t* ctx, size_t size) {
    result_t res = TVG_SUCCESS;
    tvg_point_t st, cur;
    tvg_point_t pt;
    uint32_t u32;
    float f32;
    uint8_t d;
    res = tvg_read_point(ctx, &pt);
    if (res != TVG_SUCCESS) {
        goto error;
    }
    plutovg_canvas_move_to(ctx->cvs, pt.x, pt.y);
    st = pt;
    cur = pt;
    for (size_t j = 0; j < size; ++j) {
        if (1 > ctx->inp(&d, 1, ctx->inp_state)) {
            goto error;
        }
        float line_width = 0.0f;
        if (TVG_PATH_CMD_HAS_LINE(d)) {
            res = tvg_read_unit(ctx, &line_width);
            if (res != TVG_SUCCESS) {
                goto error;
            }
        }
        switch (TVG_PATH_CMD_INDEX(d)) {
            case TVG_PATH_LINE:
                res = tvg_read_point(ctx, &pt);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
                cur = pt;
                break;
            case TVG_PATH_HLINE:
                res = tvg_read_unit(ctx, &f32);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                pt.x = f32;
                pt.y = cur.y;
                plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
                cur = pt;
                break;
            case TVG_PATH_VLINE:
                res = tvg_read_unit(ctx, &f32);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                pt.x = cur.x;
                pt.y = (float)f32;
                plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
                cur = pt;
                break;
            case TVG_PATH_CUBIC: {
                tvg_point_t ctrl1, ctrl2, endp;
                res = tvg_read_point(ctx, &ctrl1);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_point(ctx, &ctrl2);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_point(ctx, &endp);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                plutovg_canvas_cubic_to(ctx->cvs, ctrl1.x, ctrl1.y, ctrl2.x,
                                        ctrl2.y, endp.x, endp.y);
                cur = endp;
            } break;
            case TVG_PATH_ARC_CIRCLE: {
                uint8_t d;
                if (1 > ctx->inp(&d, 1, ctx->inp_state)) {
                    res = TVG_E_IO_ERROR;
                    goto error;
                }
                float radius;
                res = tvg_read_unit(ctx, &radius);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_point(ctx, &pt);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                plutovg_canvas_arc_to(ctx->cvs, radius, radius, 0,
                                      TVG_ARC_LARGE(d), 1 - TVG_ARC_SWEEP(d),
                                      pt.x, pt.y);
                cur = pt;
            } break;
            case TVG_PATH_ARC_ELLIPSE: {
                uint8_t d;
                if (1 > ctx->inp(&d, 1, ctx->inp_state)) {
                    res = TVG_E_IO_ERROR;
                    goto error;
                }
                float radius_x, radius_y;
                float rotation;
                res = tvg_read_unit(ctx, &radius_x);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_unit(ctx, &radius_y);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_unit(ctx, &rotation);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_point(ctx, &pt);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                plutovg_canvas_arc_to(
                    ctx->cvs, radius_x, radius_y, rotation * (TVG_PI / 180.0f),
                    TVG_ARC_LARGE(d), 1 - TVG_ARC_SWEEP(d), pt.x, pt.y);
                cur = pt;
            } break;
            case TVG_PATH_CLOSE:
                plutovg_canvas_close_path(ctx->cvs);
                cur = st;
                break;
            case TVG_PATH_QUAD: {
                tvg_point_t ctrl, endp;
                res = tvg_read_point(ctx, &ctrl);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                res = tvg_read_point(ctx, &endp);
                if (res != TVG_SUCCESS) {
                    goto error;
                }
                plutovg_canvas_quad_to(ctx->cvs, ctrl.x, ctrl.y, endp.x,
                                       endp.y);
                cur = endp;
            } break;
            default:
                res = TVG_E_INVALID_FORMAT;
                goto error;
        }
    }
error:
    return res;
}

static result_t tvg_parse_polyline(tvg_context_t* ctx, size_t size,
                                   const tvg_style_t* line_style,
                                   float line_width, bool close) {
    tvg_point_t pt;
    result_t res = tvg_read_point(ctx, &pt);
    if (res != TVG_SUCCESS) {
        return res;
    }
    plutovg_canvas_move_to(ctx->cvs, pt.x, pt.y);
    for (int i = 1; i < size; ++i) {
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) {
            return res;
        }
        plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
    }
    if (close) {
        plutovg_canvas_close_path(ctx->cvs);
    }
    res = tvg_apply_style(ctx, line_style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    if (line_width == 0) {  // WTH?
        line_width = .001;
    }
    plutovg_canvas_set_line_width(ctx->cvs, line_width);
    // render
    plutovg_canvas_stroke(ctx->cvs);
    return TVG_SUCCESS;
}

static result_t tvg_parse_lines(tvg_context_t* ctx, size_t size,
                                const tvg_style_t* line_style,
                                float line_width) {
    tvg_point_t pt;
    result_t res;
    for (int i = 0; i < size; ++i) {
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) {
            return res;
        }
        plutovg_canvas_move_to(ctx->cvs, pt.x, pt.y);
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) {
            return res;
        }
        plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
    }
    res = tvg_apply_style(ctx, line_style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    if (line_width == 0) {  // WTH?
        line_width = .001;
    }
    plutovg_canvas_set_line_width(ctx->cvs, line_width);
    // render
    plutovg_canvas_stroke(ctx->cvs);
    return TVG_SUCCESS;
}

static result_t tvg_parse_fill_paths(tvg_context_t* ctx, size_t size,
                                     const tvg_style_t* style) {
    result_t res = TVG_SUCCESS;
    size_t total = 0;
    uint32_t* sizes = (uint32_t*)malloc(size * sizeof(uint32_t));
    if (sizes == NULL) {
        return TVG_E_OUT_OF_MEMORY;
    }
    plutovg_canvas_set_fill_rule(ctx->cvs, PLUTOVG_FILL_RULE_EVEN_ODD);
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    plutovg_canvas_set_rgb(ctx->cvs, 0, 0, 0);
    for (size_t i = 0; i < size; ++i) {
        res = tvg_read_u32(ctx, &sizes[i]);
        ++sizes[i];
        if (res != TVG_SUCCESS) {
            goto error;
        }
        total += sizes[i];
    }
    res = tvg_apply_style(ctx, style);
    if (res != TVG_SUCCESS) {
        goto error;
    }
    // parse path
    for (size_t i = 0; i < size; ++i) {
        res = tvg_parse_path(ctx, sizes[i]);
        if (res != TVG_SUCCESS) {
            goto error;
        }
    }
    plutovg_canvas_fill(ctx->cvs);
error:
    free(sizes);
    return res;
}

static result_t tvg_parse_line_paths(tvg_context_t* ctx, size_t size,
                                     const tvg_style_t* line_style,
                                     float line_width) {
    result_t res = TVG_SUCCESS;
    size_t total = 0;
    uint32_t* sizes = (uint32_t*)malloc(size * sizeof(uint32_t));
    if (sizes == NULL) {
        return TVG_E_OUT_OF_MEMORY;
    }
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    plutovg_canvas_set_rgb(ctx->cvs, 0, 0, 0);

    for (size_t i = 0; i < size; ++i) {
        res = tvg_read_u32(ctx, &sizes[i]);
        ++sizes[i];
        if (res != TVG_SUCCESS) {
            goto error;
        }
        total += sizes[i];
    }
    res = tvg_apply_style(ctx, line_style);
    if (res != TVG_SUCCESS) {
        goto error;
    }
    // parse path
    for (size_t i = 0; i < size; ++i) {
        res = tvg_parse_path(ctx, sizes[i]);
        if (res != TVG_SUCCESS) {
            goto error;
        }
    }
    plutovg_canvas_stroke(ctx->cvs);
error:
    free(sizes);
    return res;
}

static result_t tvg_parse_outline_fill_polyline(tvg_context_t* ctx, size_t size,
                                                const tvg_style_t* fill_style,
                                                const tvg_style_t* line_style,
                                                float line_width, bool close) {
    plutovg_canvas_set_fill_rule(ctx->cvs, PLUTOVG_FILL_RULE_EVEN_ODD);
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    result_t res = tvg_apply_style(ctx, fill_style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    tvg_point_t pt;
    res = tvg_read_point(ctx, &pt);
    plutovg_canvas_move_to(ctx->cvs, pt.x, pt.y);
    for (int i = 1; i < size; ++i) {
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) {
            return res;
        }
        plutovg_canvas_line_to(ctx->cvs, pt.x, pt.y);
    }
    if (close) {
        plutovg_canvas_close_path(ctx->cvs);
    }

    plutovg_canvas_fill_preserve(ctx->cvs);
    res = tvg_apply_style(ctx, line_style);
    if (res != TVG_SUCCESS) {
        return res;
    }
    if (line_width == 0) {  // WTH?
        line_width = .001;
    }
    plutovg_canvas_set_line_width(ctx->cvs, line_width);
    // render
    plutovg_canvas_stroke(ctx->cvs);

    return res;
}

static result_t tvg_parse_outline_fill_rectangles(tvg_context_t* ctx,
                                                  size_t size,
                                                  const tvg_style_t* fill_style,
                                                  const tvg_style_t* line_style,
                                                  float line_width) {
    size_t count = size;
    size_t szb = count * sizeof(tvg_rect_t);
    result_t res;
    tvg_point_t pt;
    if (line_width == 0) {  // WTH?
        line_width = .001;
    }
    plutovg_canvas_set_fill_rule(ctx->cvs, PLUTOVG_FILL_RULE_EVEN_ODD);
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    while (count--) {
        res = tvg_read_point(ctx, &pt);
        if (res != TVG_SUCCESS) return res;
        float w, h;
        res = tvg_read_unit(ctx, &w);
        if (res != TVG_SUCCESS) return res;
        res = tvg_read_unit(ctx, &h);
        if (res != TVG_SUCCESS) return res;
        res = tvg_apply_style(ctx, fill_style);
        if (res != TVG_SUCCESS) return res;
        plutovg_canvas_rect(ctx->cvs, pt.x, pt.y, w, h);
        plutovg_canvas_fill_preserve(ctx->cvs);
        plutovg_canvas_set_line_width(ctx->cvs, line_width);
        res = tvg_apply_style(ctx, line_style);
        if (res != TVG_SUCCESS) return res;
        // render
        plutovg_canvas_stroke(ctx->cvs);
    }
    return TVG_SUCCESS;
}

static result_t tvg_parse_outline_fill_paths(tvg_context_t* ctx, size_t size,
                                             const tvg_style_t* fill_style,
                                             const tvg_style_t* line_style,
                                             float line_width) {
    result_t res = TVG_SUCCESS;

    size_t total = 0;
    uint32_t* sizes = (uint32_t*)malloc(size * sizeof(uint32_t));
    if (sizes == NULL) {
        return TVG_E_OUT_OF_MEMORY;
    }
    for (size_t i = 0; i < size; ++i) {
        res = tvg_read_u32(ctx, &sizes[i]);
        ++sizes[i];
        if (res != TVG_SUCCESS) {
            free(sizes);
            return res;
        }
        total += sizes[i];
    }
    plutovg_canvas_set_fill_rule(ctx->cvs, PLUTOVG_FILL_RULE_EVEN_ODD);
    plutovg_canvas_set_opacity(ctx->cvs, 1.0);
    res = tvg_apply_style(ctx, fill_style);

    // parse path
    for (size_t i = 0; i < size; ++i) {
        res = tvg_parse_path(ctx, sizes[i]);
        if (res != TVG_SUCCESS) {
            goto error;
        }
    }
    plutovg_canvas_fill_preserve(ctx->cvs);
    res = tvg_apply_style(ctx, line_style);
    if (res != TVG_SUCCESS) {
        goto error;
    }
    if (line_width == 0) {  // WTH?
        line_width = .001;
    }
    plutovg_canvas_set_line_width(ctx->cvs, line_width);
    // render
    plutovg_canvas_stroke(ctx->cvs);
error:
    free(sizes);
    return res;
}

static result_t tvg_parse_commands(tvg_context_t* ctx) {
    result_t res = TVG_SUCCESS;
    uint8_t cmd = 255;
    while (cmd != 0) {
        if (1 > ctx->inp(&cmd, 1, ctx->inp_state)) {
            return TVG_E_IO_ERROR;
        }
        switch (TVG_CMD_INDEX(cmd)) {
            case TVG_CMD_END_DOCUMENT:
                break;
            case TVG_CMD_FILL_POLYGON: {
                tvg_fill_header_t data;
                res =
                    tvg_parse_fill_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_fill_polygon(ctx, data.size, &data.style);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_FILL_RECTANGLES: {
                tvg_fill_header_t data;
                res =
                    tvg_parse_fill_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_fill_rectangles(ctx, data.size, &data.style);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_FILL_PATH: {
                tvg_fill_header_t data;
                res =
                    tvg_parse_fill_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_fill_paths(ctx, data.size, &data.style);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_DRAW_LINES: {
                tvg_line_header_t data;
                res =
                    tvg_parse_line_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_lines(ctx, data.size, &data.style,
                                      data.line_width);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_DRAW_LINE_LOOP: {
                tvg_line_header_t data;
                res =
                    tvg_parse_line_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_polyline(ctx, data.size, &data.style,
                                         data.line_width, true);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_DRAW_LINE_STRIP: {
                tvg_line_header_t data;
                res =
                    tvg_parse_line_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_polyline(ctx, data.size, &data.style,
                                         data.line_width, false);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_DRAW_LINE_PATH: {
                tvg_line_header_t data;
                res =
                    tvg_parse_line_header(ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_line_paths(ctx, data.size, &data.style,
                                           data.line_width);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_OUTLINE_FILL_POLYGON: {
                tvg_outline_fill_header_t data;
                res = tvg_parse_outline_fill_header(
                    ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_outline_fill_polyline(
                    ctx, data.size, &data.fill_style, &data.line_style,
                    data.line_width, true);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;
            case TVG_CMD_OUTLINE_FILL_RECTANGLES:
                tvg_outline_fill_header_t data;
                res = tvg_parse_outline_fill_header(
                    ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_outline_fill_rectangles(
                    ctx, data.size, &data.fill_style, &data.line_style,
                    data.line_width);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                break;
            case TVG_CMD_OUTLINE_FILL_PATH: {
                tvg_outline_fill_header_t data;
                res = tvg_parse_outline_fill_header(
                    ctx, TVG_CMD_STYLE_KIND(cmd), &data);
                if (res != TVG_SUCCESS) {
                    return res;
                }
                res = tvg_parse_outline_fill_paths(
                    ctx, data.size, &data.fill_style, &data.line_style,
                    data.line_width);
                if (res != TVG_SUCCESS) {
                    return res;
                }
            } break;

            default:
                return TVG_E_INVALID_FORMAT;
        }
    }
    return TVG_SUCCESS;
}

static result_t tvg_document_dimensions(tvg_input_func_t inp, void* inp_state,
                                        uint32_t* out_width,
                                        uint32_t* out_height) {
    tvg_context_t ctx;
    if (inp == NULL) {
        return TVG_E_INVALID_ARG;
    }
    ctx.inp = inp;
    ctx.inp_state = inp_state;
    ctx.colors = NULL;
    ctx.colors_size = 0;
    result_t res = tvg_parse_header(&ctx, 1);
    if (res != TVG_SUCCESS) {
        return res;
    }
    *out_width = ctx.width;
    *out_height = ctx.height;
    return res;
}

static result_t tvg_render_document(tvg_input_func_t inp, void* inp_state,
                                    plutovg_canvas_t* canvas,
                                    const plutovg_rect_t* bounds) {
    tvg_context_t ctx;
    if (inp == NULL) {
        return TVG_E_INVALID_ARG;
    }
    ctx.inp = inp;
    ctx.inp_state = inp_state;
    ctx.cvs = canvas;
    ctx.colors = NULL;
    ctx.colors_size = 0;
    result_t res = tvg_parse_header(&ctx, 0);
    if (res != TVG_SUCCESS) {
        goto error;
    }
    float scale_x = bounds->w / ctx.width;
    float scale_y = bounds->h / ctx.height;
    plutovg_matrix_t m;
    plutovg_matrix_init_scale(&m, scale_x, scale_y);
    plutovg_matrix_translate(&m, bounds->x / scale_x, bounds->y / scale_y);
    plutovg_canvas_set_matrix(ctx.cvs, &m);
    res = tvg_parse_commands(&ctx);
    if (res != TVG_SUCCESS) {
        goto error;
    }
error:
    if (ctx.colors != NULL) {
        free(ctx.colors);
        ctx.colors = NULL;
        ctx.colors_size = 0;
    }

    return res;
}

size_t inp_func(uint8_t* data, size_t to_read, void* state) {
    FILE* f = (FILE*)state;
    return fread(data, 1, to_read, f);
}

int main(int argc, char* argv[]) {
    float scale = 1.f;
    const char* input = "..\\..\\everything-32.tvg";
    const char* output = "..\\..\\output.png";
    FILE* inp_file = fopen(input, "rb");
    uint32_t w, h;
    result_t res = tvg_document_dimensions(inp_func, inp_file, &w, &h);
    if (res != TVG_SUCCESS) {
        fprintf(stderr, "Unable to parse '%s'\n", input);
        return 1;
    }

    plutovg_surface_t* surface =
        plutovg_surface_create((int)(w * scale), (int)(h * scale));
    fseek(inp_file, 0, SEEK_SET);
    plutovg_canvas_t* canvas = plutovg_canvas_create(surface);
    plutovg_rect_t r = {0.f, 0.f, w * scale, h * scale};
    res = tvg_render_document(inp_func, inp_file, canvas, &r);
    if (res != TVG_SUCCESS) {
        fprintf(stderr, "Unable to parse '%s' - error (%d)\n", input, (int)res);
    }
    if (!plutovg_surface_write_to_png(surface, output)) {
        fprintf(stderr, "Unable to write '%s'\n", output);
        goto cleanup;
    } else {
        fprintf(stderr, "Wrote '%s'\n", output);
    }

cleanup:
    fclose(inp_file);
    plutovg_canvas_destroy(canvas);
    plutovg_surface_destroy(surface);
    return 0;
}