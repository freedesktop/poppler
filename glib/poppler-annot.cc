/* poppler-annot.cc: glib interface to poppler
 *
 * Copyright (C) 2007 Inigo Martinez <inigomartinez@gmail.com>
 * Copyright (C) 2009 Carlos Garcia Campos <carlosgc@gnome.org>
 * Copyright (C) 2013 German Poo-Caamano <gpoo@gnome.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "poppler.h"
#include "poppler-private.h"

#define ZERO_CROPBOX(c) (!(c && (c->x1 > 0.01 || c->y1 > 0.01)))

const PDFRectangle *_poppler_annot_get_cropbox_and_page(PopplerAnnot *poppler_annot, Page **page_out);

/**
 * SECTION:poppler-annot
 * @short_description: Annotations
 * @title: PopplerAnnot
 */

typedef struct _PopplerAnnotClass PopplerAnnotClass;
typedef struct _PopplerAnnotMarkupClass PopplerAnnotMarkupClass;
typedef struct _PopplerAnnotFreeTextClass PopplerAnnotFreeTextClass;
typedef struct _PopplerAnnotTextClass PopplerAnnotTextClass;
typedef struct _PopplerAnnotTextMarkupClass PopplerAnnotTextMarkupClass;
typedef struct _PopplerAnnotFileAttachmentClass PopplerAnnotFileAttachmentClass;
typedef struct _PopplerAnnotMovieClass PopplerAnnotMovieClass;
typedef struct _PopplerAnnotScreenClass PopplerAnnotScreenClass;
typedef struct _PopplerAnnotLineClass PopplerAnnotLineClass;
typedef struct _PopplerAnnotCircleClass PopplerAnnotCircleClass;
typedef struct _PopplerAnnotSquareClass PopplerAnnotSquareClass;

struct _PopplerAnnotClass
{
    GObjectClass parent_class;
};

struct _PopplerAnnotMarkup
{
    PopplerAnnot parent_instance;
};

struct _PopplerAnnotMarkupClass
{
    PopplerAnnotClass parent_class;
};

struct _PopplerAnnotText
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotTextClass
{
    PopplerAnnotMarkupClass parent_class;
};

struct _PopplerAnnotTextMarkup
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotTextMarkupClass
{
    PopplerAnnotMarkupClass parent_class;
};

struct _PopplerAnnotFreeText
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotFreeTextClass
{
    PopplerAnnotMarkupClass parent_class;
};

struct _PopplerAnnotFileAttachment
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotFileAttachmentClass
{
    PopplerAnnotMarkupClass parent_class;
};

struct _PopplerAnnotMovie
{
    PopplerAnnot parent_instance;

    PopplerMovie *movie;
};

struct _PopplerAnnotMovieClass
{
    PopplerAnnotClass parent_class;
};

struct _PopplerAnnotScreen
{
    PopplerAnnot parent_instance;

    PopplerAction *action;
};

struct _PopplerAnnotScreenClass
{
    PopplerAnnotClass parent_class;
};

struct _PopplerAnnotLine
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotLineClass
{
    PopplerAnnotMarkupClass parent_class;
};

struct _PopplerAnnotCircle
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotCircleClass
{
    PopplerAnnotMarkupClass parent_class;
};

struct _PopplerAnnotSquare
{
    PopplerAnnotMarkup parent_instance;
};

struct _PopplerAnnotSquareClass
{
    PopplerAnnotMarkupClass parent_class;
};

G_DEFINE_TYPE(PopplerAnnot, poppler_annot, G_TYPE_OBJECT)
G_DEFINE_TYPE(PopplerAnnotMarkup, poppler_annot_markup, POPPLER_TYPE_ANNOT)
G_DEFINE_TYPE(PopplerAnnotTextMarkup, poppler_annot_text_markup, POPPLER_TYPE_ANNOT_MARKUP)
G_DEFINE_TYPE(PopplerAnnotText, poppler_annot_text, POPPLER_TYPE_ANNOT_MARKUP)
G_DEFINE_TYPE(PopplerAnnotFreeText, poppler_annot_free_text, POPPLER_TYPE_ANNOT_MARKUP)
G_DEFINE_TYPE(PopplerAnnotFileAttachment, poppler_annot_file_attachment, POPPLER_TYPE_ANNOT_MARKUP)
G_DEFINE_TYPE(PopplerAnnotMovie, poppler_annot_movie, POPPLER_TYPE_ANNOT)
G_DEFINE_TYPE(PopplerAnnotScreen, poppler_annot_screen, POPPLER_TYPE_ANNOT)
G_DEFINE_TYPE(PopplerAnnotLine, poppler_annot_line, POPPLER_TYPE_ANNOT_MARKUP)
G_DEFINE_TYPE(PopplerAnnotCircle, poppler_annot_circle, POPPLER_TYPE_ANNOT_MARKUP)
G_DEFINE_TYPE(PopplerAnnotSquare, poppler_annot_square, POPPLER_TYPE_ANNOT_MARKUP)

static PopplerAnnot *_poppler_create_annot(GType annot_type, Annot *annot)
{
    PopplerAnnot *poppler_annot;

    poppler_annot = POPPLER_ANNOT(g_object_new(annot_type, nullptr));
    poppler_annot->annot = annot;
    annot->incRefCnt();

    return poppler_annot;
}

static void poppler_annot_finalize(GObject *object)
{
    PopplerAnnot *poppler_annot = POPPLER_ANNOT(object);

    if (poppler_annot->annot) {
        poppler_annot->annot->decRefCnt();
        poppler_annot->annot = nullptr;
    }

    G_OBJECT_CLASS(poppler_annot_parent_class)->finalize(object);
}

static void poppler_annot_init(PopplerAnnot *poppler_annot) { }

static void poppler_annot_class_init(PopplerAnnotClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->finalize = poppler_annot_finalize;
}

PopplerAnnot *_poppler_annot_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT, annot);
}

static void poppler_annot_markup_init(PopplerAnnotMarkup *poppler_annot) { }

static void poppler_annot_markup_class_init(PopplerAnnotMarkupClass *klass) { }

static void poppler_annot_text_init(PopplerAnnotText *poppler_annot) { }

static void poppler_annot_text_class_init(PopplerAnnotTextClass *klass) { }

PopplerAnnot *_poppler_annot_text_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_TEXT, annot);
}

/**
 * poppler_annot_text_new:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 *
 * Creates a new Text annotation that will be
 * located on @rect when added to a page. See
 * poppler_page_add_annot()
 *
 * Return value: A newly created #PopplerAnnotText annotation
 *
 * Since: 0.16
 */
PopplerAnnot *poppler_annot_text_new(PopplerDocument *doc, PopplerRectangle *rect)
{
    Annot *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    annot = new AnnotText(doc->doc, &pdf_rect);

    return _poppler_annot_text_new(annot);
}

PopplerAnnot *_poppler_annot_text_markup_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_TEXT_MARKUP, annot);
}

static AnnotQuadrilaterals *create_annot_quads_from_poppler_quads(GArray *quads)
{
    g_assert(quads->len > 0);

    auto quads_array = std::make_unique<AnnotQuadrilaterals::AnnotQuadrilateral[]>(quads->len);
    for (guint i = 0; i < quads->len; i++) {
        PopplerQuadrilateral *quadrilateral = &g_array_index(quads, PopplerQuadrilateral, i);

        quads_array[i] = AnnotQuadrilaterals::AnnotQuadrilateral(quadrilateral->p1.x, quadrilateral->p1.y, quadrilateral->p2.x, quadrilateral->p2.y, quadrilateral->p3.x, quadrilateral->p3.y, quadrilateral->p4.x, quadrilateral->p4.y);
    }

    return new AnnotQuadrilaterals(std::move(quads_array), quads->len);
}

/* If @crop_box parameter is non null, it will subtract the crop_box offset
 * from the coordinates of the returned #PopplerQuadrilateral array */
static GArray *create_poppler_quads_from_annot_quads(AnnotQuadrilaterals *quads_array, const PDFRectangle *crop_box)
{
    GArray *quads;
    guint quads_len;
    PDFRectangle zerobox;

    if (!crop_box) {
        zerobox = PDFRectangle();
        crop_box = &zerobox;
    }

    quads_len = quads_array->getQuadrilateralsLength();
    quads = g_array_sized_new(FALSE, FALSE, sizeof(PopplerQuadrilateral), quads_len);
    g_array_set_size(quads, quads_len);

    for (guint i = 0; i < quads_len; ++i) {
        PopplerQuadrilateral *quadrilateral = &g_array_index(quads, PopplerQuadrilateral, i);

        quadrilateral->p1.x = quads_array->getX1(i) - crop_box->x1;
        quadrilateral->p1.y = quads_array->getY1(i) - crop_box->y1;
        quadrilateral->p2.x = quads_array->getX2(i) - crop_box->x1;
        quadrilateral->p2.y = quads_array->getY2(i) - crop_box->y1;
        quadrilateral->p3.x = quads_array->getX3(i) - crop_box->x1;
        quadrilateral->p3.y = quads_array->getY3(i) - crop_box->y1;
        quadrilateral->p4.x = quads_array->getX4(i) - crop_box->x1;
        quadrilateral->p4.y = quads_array->getY4(i) - crop_box->y1;
    }

    return quads;
}

static void poppler_annot_text_markup_init(PopplerAnnotTextMarkup *poppler_annot) { }

static void poppler_annot_text_markup_class_init(PopplerAnnotTextMarkupClass *klass) { }

/**
 * poppler_annot_text_markup_new_highlight:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 * @quadrilaterals: (element-type PopplerQuadrilateral): A #GArray of
 *   #PopplerQuadrilateral<!-- -->s
 *
 * Creates a new Highlight Text annotation that will be
 * located on @rect when added to a page. See poppler_page_add_annot()
 *
 * Return value: (transfer full): A newly created #PopplerAnnotTextMarkup annotation
 *
 * Since: 0.26
 */
PopplerAnnot *poppler_annot_text_markup_new_highlight(PopplerDocument *doc, PopplerRectangle *rect, GArray *quadrilaterals)
{
    PopplerAnnot *poppler_annot;
    AnnotTextMarkup *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    annot = new AnnotTextMarkup(doc->doc, &pdf_rect, Annot::typeHighlight);

    poppler_annot = _poppler_annot_text_markup_new(annot);
    poppler_annot_text_markup_set_quadrilaterals(POPPLER_ANNOT_TEXT_MARKUP(poppler_annot), quadrilaterals);
    return poppler_annot;
}

/**
 * poppler_annot_text_markup_new_squiggly:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 * @quadrilaterals: (element-type PopplerQuadrilateral): A #GArray of
 *   #PopplerQuadrilateral<!-- -->s
 *
 * Creates a new Squiggly Text annotation that will be
 * located on @rect when added to a page. See poppler_page_add_annot()
 *
 * Return value: (transfer full): A newly created #PopplerAnnotTextMarkup annotation
 *
 * Since: 0.26
 */
PopplerAnnot *poppler_annot_text_markup_new_squiggly(PopplerDocument *doc, PopplerRectangle *rect, GArray *quadrilaterals)
{
    PopplerAnnot *poppler_annot;
    AnnotTextMarkup *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    g_return_val_if_fail(quadrilaterals != nullptr && quadrilaterals->len > 0, NULL);

    annot = new AnnotTextMarkup(doc->doc, &pdf_rect, Annot::typeSquiggly);

    poppler_annot = _poppler_annot_text_markup_new(annot);
    poppler_annot_text_markup_set_quadrilaterals(POPPLER_ANNOT_TEXT_MARKUP(poppler_annot), quadrilaterals);
    return poppler_annot;
}

/**
 * poppler_annot_text_markup_new_strikeout:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 * @quadrilaterals: (element-type PopplerQuadrilateral): A #GArray of
 *   #PopplerQuadrilateral<!-- -->s
 *
 * Creates a new Strike Out Text annotation that will be
 * located on @rect when added to a page. See poppler_page_add_annot()
 *
 * Return value: (transfer full): A newly created #PopplerAnnotTextMarkup annotation
 *
 * Since: 0.26
 */
PopplerAnnot *poppler_annot_text_markup_new_strikeout(PopplerDocument *doc, PopplerRectangle *rect, GArray *quadrilaterals)
{
    PopplerAnnot *poppler_annot;
    AnnotTextMarkup *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    g_return_val_if_fail(quadrilaterals != nullptr && quadrilaterals->len > 0, NULL);

    annot = new AnnotTextMarkup(doc->doc, &pdf_rect, Annot::typeStrikeOut);

    poppler_annot = _poppler_annot_text_markup_new(annot);
    poppler_annot_text_markup_set_quadrilaterals(POPPLER_ANNOT_TEXT_MARKUP(poppler_annot), quadrilaterals);
    return poppler_annot;
}

/**
 * poppler_annot_text_markup_new_underline:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 * @quadrilaterals: (element-type PopplerQuadrilateral): A #GArray of
 *   #PopplerQuadrilateral<!-- -->s
 *
 * Creates a new Underline Text annotation that will be
 * located on @rect when added to a page. See poppler_page_add_annot()
 *
 * Return value: (transfer full): A newly created #PopplerAnnotTextMarkup annotation
 *
 * Since: 0.26
 */
PopplerAnnot *poppler_annot_text_markup_new_underline(PopplerDocument *doc, PopplerRectangle *rect, GArray *quadrilaterals)
{
    PopplerAnnot *poppler_annot;
    AnnotTextMarkup *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    g_return_val_if_fail(quadrilaterals != nullptr && quadrilaterals->len > 0, NULL);

    annot = new AnnotTextMarkup(doc->doc, &pdf_rect, Annot::typeUnderline);

    poppler_annot = _poppler_annot_text_markup_new(annot);
    poppler_annot_text_markup_set_quadrilaterals(POPPLER_ANNOT_TEXT_MARKUP(poppler_annot), quadrilaterals);
    return poppler_annot;
}

static void poppler_annot_free_text_init(PopplerAnnotFreeText *poppler_annot) { }

static void poppler_annot_free_text_class_init(PopplerAnnotFreeTextClass *klass) { }

PopplerAnnot *_poppler_annot_free_text_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_FREE_TEXT, annot);
}

static void poppler_annot_file_attachment_init(PopplerAnnotFileAttachment *poppler_annot) { }

static void poppler_annot_file_attachment_class_init(PopplerAnnotFileAttachmentClass *klass) { }

PopplerAnnot *_poppler_annot_file_attachment_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_FILE_ATTACHMENT, annot);
}

static void poppler_annot_movie_finalize(GObject *object)
{
    PopplerAnnotMovie *annot_movie = POPPLER_ANNOT_MOVIE(object);

    if (annot_movie->movie) {
        g_object_unref(annot_movie->movie);
        annot_movie->movie = nullptr;
    }

    G_OBJECT_CLASS(poppler_annot_movie_parent_class)->finalize(object);
}

static void poppler_annot_movie_init(PopplerAnnotMovie *poppler_annot) { }

static void poppler_annot_movie_class_init(PopplerAnnotMovieClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->finalize = poppler_annot_movie_finalize;
}

PopplerAnnot *_poppler_annot_movie_new(Annot *annot)
{
    PopplerAnnot *poppler_annot;
    AnnotMovie *annot_movie;

    poppler_annot = _poppler_create_annot(POPPLER_TYPE_ANNOT_MOVIE, annot);
    annot_movie = static_cast<AnnotMovie *>(poppler_annot->annot);
    POPPLER_ANNOT_MOVIE(poppler_annot)->movie = _poppler_movie_new(annot_movie->getMovie());

    return poppler_annot;
}

static void poppler_annot_screen_finalize(GObject *object)
{
    PopplerAnnotScreen *annot_screen = POPPLER_ANNOT_SCREEN(object);

    if (annot_screen->action) {
        poppler_action_free(annot_screen->action);
        annot_screen->action = nullptr;
    }

    G_OBJECT_CLASS(poppler_annot_screen_parent_class)->finalize(object);
}

static void poppler_annot_screen_init(PopplerAnnotScreen *poppler_annot) { }

static void poppler_annot_screen_class_init(PopplerAnnotScreenClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->finalize = poppler_annot_screen_finalize;
}

PopplerAnnot *_poppler_annot_screen_new(PopplerDocument *doc, Annot *annot)
{
    PopplerAnnot *poppler_annot;
    AnnotScreen *annot_screen;
    LinkAction *action;

    poppler_annot = _poppler_create_annot(POPPLER_TYPE_ANNOT_SCREEN, annot);
    annot_screen = static_cast<AnnotScreen *>(poppler_annot->annot);
    action = annot_screen->getAction();
    if (action)
        POPPLER_ANNOT_SCREEN(poppler_annot)->action = _poppler_action_new(doc, action, nullptr);

    return poppler_annot;
}

PopplerAnnot *_poppler_annot_line_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_LINE, annot);
}

static void poppler_annot_line_init(PopplerAnnotLine *poppler_annot) { }

static void poppler_annot_line_class_init(PopplerAnnotLineClass *klass) { }

/**
 * poppler_annot_line_new:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 * @start: a #PopplerPoint of the starting vertice
 * @end: a #PopplerPoint of the ending vertice
 *
 * Creates a new Line annotation that will be
 * located on @rect when added to a page. See
 * poppler_page_add_annot()
 *
 * Return value: A newly created #PopplerAnnotLine annotation
 *
 * Since: 0.26
 */
PopplerAnnot *poppler_annot_line_new(PopplerDocument *doc, PopplerRectangle *rect, PopplerPoint *start, PopplerPoint *end)
{
    PopplerAnnot *poppler_annot;
    Annot *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    annot = new AnnotLine(doc->doc, &pdf_rect);

    poppler_annot = _poppler_annot_line_new(annot);
    poppler_annot_line_set_vertices(POPPLER_ANNOT_LINE(poppler_annot), start, end);
    return poppler_annot;
}

PopplerAnnot *_poppler_annot_circle_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_CIRCLE, annot);
}

static void poppler_annot_circle_init(PopplerAnnotCircle *poppler_annot) { }

static void poppler_annot_circle_class_init(PopplerAnnotCircleClass *klass) { }

/**
 * poppler_annot_circle_new:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 *
 * Creates a new Circle annotation that will be
 * located on @rect when added to a page. See
 * poppler_page_add_annot()
 *
 * Return value: a newly created #PopplerAnnotCircle annotation
 *
 * Since: 0.26
 **/
PopplerAnnot *poppler_annot_circle_new(PopplerDocument *doc, PopplerRectangle *rect)
{
    Annot *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    annot = new AnnotGeometry(doc->doc, &pdf_rect, Annot::typeCircle);

    return _poppler_annot_circle_new(annot);
}

PopplerAnnot *_poppler_annot_square_new(Annot *annot)
{
    return _poppler_create_annot(POPPLER_TYPE_ANNOT_SQUARE, annot);
}

static void poppler_annot_square_init(PopplerAnnotSquare *poppler_annot) { }

static void poppler_annot_square_class_init(PopplerAnnotSquareClass *klass) { }

/**
 * poppler_annot_square_new:
 * @doc: a #PopplerDocument
 * @rect: a #PopplerRectangle
 *
 * Creates a new Square annotation that will be
 * located on @rect when added to a page. See
 * poppler_page_add_annot()
 *
 * Return value: a newly created #PopplerAnnotSquare annotation
 *
 * Since: 0.26
 **/
PopplerAnnot *poppler_annot_square_new(PopplerDocument *doc, PopplerRectangle *rect)
{
    Annot *annot;
    PDFRectangle pdf_rect(rect->x1, rect->y1, rect->x2, rect->y2);

    annot = new AnnotGeometry(doc->doc, &pdf_rect, Annot::typeSquare);

    return _poppler_annot_square_new(annot);
}

/* Public methods */
/**
 * poppler_annot_get_annot_type:
 * @poppler_annot: a #PopplerAnnot
 *
 * Gets the type of @poppler_annot
 *
 * Return value: #PopplerAnnotType of @poppler_annot.
 **/
PopplerAnnotType poppler_annot_get_annot_type(PopplerAnnot *poppler_annot)
{
    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), POPPLER_ANNOT_UNKNOWN);

    switch (poppler_annot->annot->getType()) {
    case Annot::typeText:
        return POPPLER_ANNOT_TEXT;
    case Annot::typeLink:
        return POPPLER_ANNOT_LINK;
    case Annot::typeFreeText:
        return POPPLER_ANNOT_FREE_TEXT;
    case Annot::typeLine:
        return POPPLER_ANNOT_LINE;
    case Annot::typeSquare:
        return POPPLER_ANNOT_SQUARE;
    case Annot::typeCircle:
        return POPPLER_ANNOT_CIRCLE;
    case Annot::typePolygon:
        return POPPLER_ANNOT_POLYGON;
    case Annot::typePolyLine:
        return POPPLER_ANNOT_POLY_LINE;
    case Annot::typeHighlight:
        return POPPLER_ANNOT_HIGHLIGHT;
    case Annot::typeUnderline:
        return POPPLER_ANNOT_UNDERLINE;
    case Annot::typeSquiggly:
        return POPPLER_ANNOT_SQUIGGLY;
    case Annot::typeStrikeOut:
        return POPPLER_ANNOT_STRIKE_OUT;
    case Annot::typeStamp:
        return POPPLER_ANNOT_STAMP;
    case Annot::typeCaret:
        return POPPLER_ANNOT_CARET;
    case Annot::typeInk:
        return POPPLER_ANNOT_INK;
    case Annot::typePopup:
        return POPPLER_ANNOT_POPUP;
    case Annot::typeFileAttachment:
        return POPPLER_ANNOT_FILE_ATTACHMENT;
    case Annot::typeSound:
        return POPPLER_ANNOT_SOUND;
    case Annot::typeMovie:
        return POPPLER_ANNOT_MOVIE;
    case Annot::typeWidget:
        return POPPLER_ANNOT_WIDGET;
    case Annot::typeScreen:
        return POPPLER_ANNOT_SCREEN;
    case Annot::typePrinterMark:
        return POPPLER_ANNOT_PRINTER_MARK;
    case Annot::typeTrapNet:
        return POPPLER_ANNOT_TRAP_NET;
    case Annot::typeWatermark:
        return POPPLER_ANNOT_WATERMARK;
    case Annot::type3D:
        return POPPLER_ANNOT_3D;
    default:
        g_warning("Unsupported Annot Type");
    }

    return POPPLER_ANNOT_UNKNOWN;
}

/**
 * poppler_annot_get_contents:
 * @poppler_annot: a #PopplerAnnot
 *
 * Retrieves the contents of @poppler_annot.
 *
 * Return value: a new allocated string with the contents of @poppler_annot. It
 *               must be freed with g_free() when done.
 **/
gchar *poppler_annot_get_contents(PopplerAnnot *poppler_annot)
{
    const GooString *contents;

    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), NULL);

    contents = poppler_annot->annot->getContents();

    return contents && contents->getLength() > 0 ? _poppler_goo_string_to_utf8(contents) : nullptr;
}

/**
 * poppler_annot_set_contents:
 * @poppler_annot: a #PopplerAnnot
 * @contents: a text string containing the new contents
 *
 * Sets the contents of @poppler_annot to the given value,
 * replacing the current contents.
 *
 * Since: 0.12
 **/
void poppler_annot_set_contents(PopplerAnnot *poppler_annot, const gchar *contents)
{
    GooString *goo_tmp;
    gchar *tmp;
    gsize length = 0;

    g_return_if_fail(POPPLER_IS_ANNOT(poppler_annot));

    tmp = contents ? g_convert(contents, -1, "UTF-16BE", "UTF-8", nullptr, &length, nullptr) : nullptr;
    goo_tmp = new GooString(tmp, length);
    g_free(tmp);
    poppler_annot->annot->setContents(goo_tmp);
    delete (goo_tmp);
}

/**
 * poppler_annot_get_name:
 * @poppler_annot: a #PopplerAnnot
 *
 * Retrieves the name of @poppler_annot.
 *
 * Return value: a new allocated string with the name of @poppler_annot. It must
 *               be freed with g_free() when done.
 **/
gchar *poppler_annot_get_name(PopplerAnnot *poppler_annot)
{
    const GooString *name;

    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), NULL);

    name = poppler_annot->annot->getName();

    return name ? _poppler_goo_string_to_utf8(name) : nullptr;
}

/**
 * poppler_annot_get_modified:
 * @poppler_annot: a #PopplerAnnot
 *
 * Retrieves the last modification data of @poppler_annot. The returned
 * string will be either a PDF format date or a text string.
 * See also #poppler_date_parse()
 *
 * Return value: a new allocated string with the last modification data of
 *               @poppler_annot. It must be freed with g_free() when done.
 **/
gchar *poppler_annot_get_modified(PopplerAnnot *poppler_annot)
{
    const GooString *text;

    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), NULL);

    text = poppler_annot->annot->getModified();

    return text ? _poppler_goo_string_to_utf8(text) : nullptr;
}

/**
 * poppler_annot_get_flags:
 * @poppler_annot: a #PopplerAnnot
 *
 * Retrieves the flag field specifying various characteristics of the
 * @poppler_annot.
 *
 * Return value: the flag field of @poppler_annot.
 **/
PopplerAnnotFlag poppler_annot_get_flags(PopplerAnnot *poppler_annot)
{
    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), (PopplerAnnotFlag)0);

    return (PopplerAnnotFlag)poppler_annot->annot->getFlags();
}

/**
 * poppler_annot_set_flags:
 * @poppler_annot: a #PopplerAnnot
 * @flags: a #PopplerAnnotFlag
 *
 * Sets the flag field specifying various characteristics of the
 * @poppler_annot.
 *
 * Since: 0.22
 **/
void poppler_annot_set_flags(PopplerAnnot *poppler_annot, PopplerAnnotFlag flags)
{
    g_return_if_fail(POPPLER_IS_ANNOT(poppler_annot));

    if (poppler_annot_get_flags(poppler_annot) == flags)
        return;

    poppler_annot->annot->setFlags((guint)flags);
}

static PopplerColor *create_poppler_color_from_annot_color(AnnotColor *color)
{
    PopplerColor *poppler_color = nullptr;

    if (color) {
        const double *values = color->getValues();

        switch (color->getSpace()) {
        case AnnotColor::colorGray:
            poppler_color = g_new(PopplerColor, 1);

            poppler_color->red = (guint16)(values[0] * 65535);
            poppler_color->green = poppler_color->red;
            poppler_color->blue = poppler_color->red;

            break;
        case AnnotColor::colorRGB:
            poppler_color = g_new(PopplerColor, 1);

            poppler_color->red = (guint16)(values[0] * 65535);
            poppler_color->green = (guint16)(values[1] * 65535);
            poppler_color->blue = (guint16)(values[2] * 65535);

            break;
        case AnnotColor::colorCMYK:
            g_warning("Unsupported Annot Color: colorCMYK");
        case AnnotColor::colorTransparent:
            break;
        }
    }

    return poppler_color;
}

static std::unique_ptr<AnnotColor> create_annot_color_from_poppler_color(PopplerColor *poppler_color)
{
    if (!poppler_color)
        return nullptr;

    return std::make_unique<AnnotColor>((double)poppler_color->red / 65535, (double)poppler_color->green / 65535, (double)poppler_color->blue / 65535);
}

/**
 * poppler_annot_get_color:
 * @poppler_annot: a #PopplerAnnot
 *
 * Retrieves the color of @poppler_annot.
 *
 * Return value: a new allocated #PopplerColor with the color values of
 *               @poppler_annot, or %NULL. It must be freed with g_free() when done.
 **/
PopplerColor *poppler_annot_get_color(PopplerAnnot *poppler_annot)
{
    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), NULL);

    return create_poppler_color_from_annot_color(poppler_annot->annot->getColor());
}

/**
 * poppler_annot_set_color:
 * @poppler_annot: a #PopplerAnnot
 * @poppler_color: (allow-none): a #PopplerColor, or %NULL
 *
 * Sets the color of @poppler_annot.
 *
 * Since: 0.16
 */
void poppler_annot_set_color(PopplerAnnot *poppler_annot, PopplerColor *poppler_color)
{
    poppler_annot->annot->setColor(create_annot_color_from_poppler_color(poppler_color));
}

/**
 * poppler_annot_get_page_index:
 * @poppler_annot: a #PopplerAnnot
 *
 * Returns the page index to which @poppler_annot is associated, or -1 if unknown
 *
 * Return value: page index or -1
 *
 * Since: 0.14
 **/
gint poppler_annot_get_page_index(PopplerAnnot *poppler_annot)
{
    gint page_num;

    g_return_val_if_fail(POPPLER_IS_ANNOT(poppler_annot), -1);

    page_num = poppler_annot->annot->getPageNum();
    return page_num <= 0 ? -1 : page_num - 1;
}

/* Returns cropbox rect for the page where the passed in @poppler_annot is in,
 * or NULL when could not retrieve the cropbox. If @page_out is non-null then
 * it will be set with the page that @poppler_annot is in. */
const PDFRectangle *_poppler_annot_get_cropbox_and_page(PopplerAnnot *poppler_annot, Page **page_out)
{
    int page_index;

    /* A returned zero means annot is not added to any page yet */
    page_index = poppler_annot->annot->getPageNum();

    if (page_index) {
        Page *page;

        page = poppler_annot->annot->getDoc()->getPage(page_index);
        if (page) {
            if (page_out)
                *page_out = page;

            return page->getCropBox();
        }
    }

    return nullptr;
}

/* Returns cropbox rect for the page where the passed in @poppler_annot is in,
 * or NULL when could not retrieve the cropbox */
const PDFRectangle *_poppler_annot_get_cropbox(PopplerAnnot *poppler_annot)
{
    return _poppler_annot_get_cropbox_and_page(poppler_annot, nullptr);
}

/**
 * poppler_annot_get_rectangle:
 * @poppler_annot: a #PopplerAnnot
 * @poppler_rect: (out): a #PopplerRectangle to store the annotation's coordinates
 *
 * Retrieves the rectangle representing the page coordinates where the
 * annotation @poppler_annot is placed.
 *
 * Since: 0.26
 */
void poppler_annot_get_rectangle(PopplerAnnot *poppler_annot, PopplerRectangle *poppler_rect)
{
    PDFRectangle *annot_rect;
    const PDFRectangle *crop_box;
    PDFRectangle zerobox;
    Page *page = nullptr;

    g_return_if_fail(POPPLER_IS_ANNOT(poppler_annot));
    g_return_if_fail(poppler_rect != nullptr);

    crop_box = _poppler_annot_get_cropbox_and_page(poppler_annot, &page);
    if (!crop_box) {
        zerobox = PDFRectangle();
        crop_box = &zerobox;
    }

    annot_rect = poppler_annot->annot->getRect();
    poppler_rect->x1 = annot_rect->x1 - crop_box->x1;
    poppler_rect->x2 = annot_rect->x2 - crop_box->x1;
    poppler_rect->y1 = annot_rect->y1 - crop_box->y1;
    poppler_rect->y2 = annot_rect->y2 - crop_box->y1;
}

/**
 * poppler_annot_set_rectangle:
 * @poppler_annot: a #PopplerAnnot
 * @poppler_rect: a #PopplerRectangle with the new annotation's coordinates
 *
 * Move the annotation to the rectangle representing the page coordinates
 * where the annotation @poppler_annot should be placed.
 *
 * Since: 0.26
 */
void poppler_annot_set_rectangle(PopplerAnnot *poppler_annot, PopplerRectangle *poppler_rect)
{
    const PDFRectangle *crop_box;
    PDFRectangle zerobox;
    double x1, y1, x2, y2;
    Page *page = nullptr;

    g_return_if_fail(POPPLER_IS_ANNOT(poppler_annot));
    g_return_if_fail(poppler_rect != nullptr);

    crop_box = _poppler_annot_get_cropbox_and_page(poppler_annot, &page);
    if (!crop_box) {
        zerobox = PDFRectangle();
        crop_box = &zerobox;
    }

    x1 = poppler_rect->x1;
    y1 = poppler_rect->y1;
    x2 = poppler_rect->x2;
    y2 = poppler_rect->y2;

    if (page && SUPPORTED_ROTATION(page->getRotate())) {
        /* annot is inside a rotated page, as core poppler rect must be saved
         * un-rotated, let's proceed to un-rotate rect before saving */
        _unrotate_rect_for_annot_and_page(page, poppler_annot->annot, &x1, &y1, &x2, &y2);
    }

    poppler_annot->annot->setRect(x1 + crop_box->x1, y1 + crop_box->y1, x2 + crop_box->x1, y2 + crop_box->y1);
}

/* PopplerAnnotMarkup */
/**
 * poppler_annot_markup_get_label:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Retrieves the label text of @poppler_annot.
 *
 * Return value: the label text of @poppler_annot.
 */
gchar *poppler_annot_markup_get_label(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;
    const GooString *text;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), NULL);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    text = annot->getLabel();

    return text ? _poppler_goo_string_to_utf8(text) : nullptr;
}

/**
 * poppler_annot_markup_set_label:
 * @poppler_annot: a #PopplerAnnotMarkup
 * @label: (allow-none): a text string containing the new label, or %NULL
 *
 * Sets the label text of @poppler_annot, replacing the current one
 *
 * Since: 0.16
 */
void poppler_annot_markup_set_label(PopplerAnnotMarkup *poppler_annot, const gchar *label)
{
    AnnotMarkup *annot;
    GooString *goo_tmp;
    gchar *tmp;
    gsize length = 0;

    g_return_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot));

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    tmp = label ? g_convert(label, -1, "UTF-16BE", "UTF-8", nullptr, &length, nullptr) : nullptr;
    goo_tmp = new GooString(tmp, length);
    g_free(tmp);
    annot->setLabel(goo_tmp);
    delete goo_tmp;
}

/**
 * poppler_annot_markup_has_popup:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Return %TRUE if the markup annotation has a popup window associated
 *
 * Return value: %TRUE, if @poppler_annot has popup, %FALSE otherwise
 *
 * Since: 0.12
 **/
gboolean poppler_annot_markup_has_popup(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), FALSE);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    return annot->getPopup() != nullptr;
}

/**
 * poppler_annot_markup_set_popup:
 * @poppler_annot: a #PopplerAnnotMarkup
 * @popup_rect: a #PopplerRectangle
 *
 * Associates a new popup window for editing contents of @poppler_annot.
 * Popup window shall be displayed by viewers at @popup_rect on the page.
 *
 * Since: 0.16
 */
void poppler_annot_markup_set_popup(PopplerAnnotMarkup *poppler_annot, PopplerRectangle *popup_rect)
{
    AnnotMarkup *annot;
    PDFRectangle pdf_rect(popup_rect->x1, popup_rect->y1, popup_rect->x2, popup_rect->y2);

    g_return_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot));

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot->setPopup(std::make_unique<AnnotPopup>(annot->getDoc(), &pdf_rect));
}

/**
 * poppler_annot_markup_get_popup_is_open:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Retrieves the state of the popup window related to @poppler_annot.
 *
 * Return value: the state of @poppler_annot. %TRUE if it's open, %FALSE in
 *               other case.
 **/
gboolean poppler_annot_markup_get_popup_is_open(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;
    AnnotPopup *annot_popup;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), FALSE);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    if ((annot_popup = annot->getPopup()))
        return annot_popup->getOpen();

    return FALSE;
}

/**
 * poppler_annot_markup_set_popup_is_open:
 * @poppler_annot: a #PopplerAnnotMarkup
 * @is_open: whether popup window should initially be displayed open
 *
 * Sets the state of the popup window related to @poppler_annot.
 *
 * Since: 0.16
 **/
void poppler_annot_markup_set_popup_is_open(PopplerAnnotMarkup *poppler_annot, gboolean is_open)
{
    AnnotMarkup *annot;
    AnnotPopup *annot_popup;

    g_return_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot));

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    annot_popup = annot->getPopup();
    if (!annot_popup)
        return;

    if (annot_popup->getOpen() != is_open)
        annot_popup->setOpen(is_open);
}

/**
 * poppler_annot_markup_get_popup_rectangle:
 * @poppler_annot: a #PopplerAnnotMarkup
 * @poppler_rect: (out): a #PopplerRectangle to store the popup rectangle
 *
 * Retrieves the rectangle of the popup window related to @poppler_annot.
 *
 * Return value: %TRUE if #PopplerRectangle was correctly filled, %FALSE otherwise
 *
 * Since: 0.12
 **/
gboolean poppler_annot_markup_get_popup_rectangle(PopplerAnnotMarkup *poppler_annot, PopplerRectangle *poppler_rect)
{
    AnnotMarkup *annot;
    Annot *annot_popup;
    PDFRectangle *annot_rect;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), FALSE);
    g_return_val_if_fail(poppler_rect != nullptr, FALSE);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot_popup = annot->getPopup();
    if (!annot_popup)
        return FALSE;

    annot_rect = annot_popup->getRect();
    poppler_rect->x1 = annot_rect->x1;
    poppler_rect->x2 = annot_rect->x2;
    poppler_rect->y1 = annot_rect->y1;
    poppler_rect->y2 = annot_rect->y2;

    return TRUE;
}

/**
 * poppler_annot_markup_set_popup_rectangle:
 * @poppler_annot: a #PopplerAnnotMarkup
 * @poppler_rect: a #PopplerRectangle to set
 *
 * Sets the rectangle of the popup window related to @poppler_annot.
 * This doesn't have any effect if @poppler_annot doesn't have a
 * popup associated, use poppler_annot_markup_set_popup() to associate
 * a popup window to a #PopplerAnnotMarkup.
 *
 * Since: 0.33
 */
void poppler_annot_markup_set_popup_rectangle(PopplerAnnotMarkup *poppler_annot, PopplerRectangle *poppler_rect)
{
    AnnotMarkup *annot;
    Annot *annot_popup;

    g_return_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot));
    g_return_if_fail(poppler_rect != nullptr);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot_popup = annot->getPopup();
    if (!annot_popup)
        return;

    annot_popup->setRect(poppler_rect->x1, poppler_rect->y1, poppler_rect->x2, poppler_rect->y2);
}

/**
 * poppler_annot_markup_get_opacity:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Retrieves the opacity value of @poppler_annot.
 *
 * Return value: the opacity value of @poppler_annot,
 *               between 0 (transparent) and 1 (opaque)
 */
gdouble poppler_annot_markup_get_opacity(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), 0);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    return annot->getOpacity();
}

/**
 * poppler_annot_markup_set_opacity:
 * @poppler_annot: a #PopplerAnnotMarkup
 * @opacity: a constant opacity value, between 0 (transparent) and 1 (opaque)
 *
 * Sets the opacity of @poppler_annot. This value applies to
 * all visible elements of @poppler_annot in its closed state,
 * but not to the pop-up window that appears when it's openened
 *
 * Since: 0.16
 */
void poppler_annot_markup_set_opacity(PopplerAnnotMarkup *poppler_annot, gdouble opacity)
{
    AnnotMarkup *annot;

    g_return_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot));

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot->setOpacity(opacity);
}

/**
 * poppler_annot_markup_get_date:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Returns the date and time when the annotation was created
 *
 * Return value: (transfer full): a #GDate representing the date and time
 *               when the annotation was created, or %NULL
 */
GDate *poppler_annot_markup_get_date(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;
    const GooString *annot_date;
    time_t timet;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), NULL);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot_date = annot->getDate();
    if (!annot_date)
        return nullptr;

    if (_poppler_convert_pdf_date_to_gtime(annot_date, &timet)) {
        GDate *date;

        date = g_date_new();
        g_date_set_time_t(date, timet);

        return date;
    }

    return nullptr;
}

/**
 * poppler_annot_markup_get_subject:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Retrives the subject text of @poppler_annot.
 *
 * Return value: the subject text of @poppler_annot.
 */
gchar *poppler_annot_markup_get_subject(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;
    const GooString *text;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), NULL);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    text = annot->getSubject();

    return text ? _poppler_goo_string_to_utf8(text) : nullptr;
}

/**
 * poppler_annot_markup_get_reply_to:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Gets the reply type of @poppler_annot.
 *
 * Return value: #PopplerAnnotMarkupReplyType of @poppler_annot.
 */
PopplerAnnotMarkupReplyType poppler_annot_markup_get_reply_to(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), POPPLER_ANNOT_MARKUP_REPLY_TYPE_R);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    switch (annot->getReplyTo()) {
    case AnnotMarkup::replyTypeR:
        return POPPLER_ANNOT_MARKUP_REPLY_TYPE_R;
    case AnnotMarkup::replyTypeGroup:
        return POPPLER_ANNOT_MARKUP_REPLY_TYPE_GROUP;
    default:
        g_warning("Unsupported Annot Markup Reply To Type");
    }

    return POPPLER_ANNOT_MARKUP_REPLY_TYPE_R;
}

/**
 * poppler_annot_markup_get_external_data:
 * @poppler_annot: a #PopplerAnnotMarkup
 *
 * Gets the external data type of @poppler_annot.
 *
 * Return value: #PopplerAnnotExternalDataType of @poppler_annot.
 */
PopplerAnnotExternalDataType poppler_annot_markup_get_external_data(PopplerAnnotMarkup *poppler_annot)
{
    AnnotMarkup *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MARKUP(poppler_annot), POPPLER_ANNOT_EXTERNAL_DATA_MARKUP_UNKNOWN);

    annot = static_cast<AnnotMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);

    switch (annot->getExData()) {
    case annotExternalDataMarkup3D:
        return POPPLER_ANNOT_EXTERNAL_DATA_MARKUP_3D;
    case annotExternalDataMarkupUnknown:
        return POPPLER_ANNOT_EXTERNAL_DATA_MARKUP_UNKNOWN;
    default:
        g_warning("Unsupported Annot Markup External Data");
    }

    return POPPLER_ANNOT_EXTERNAL_DATA_MARKUP_UNKNOWN;
}

/* PopplerAnnotText */
/**
 * poppler_annot_text_get_is_open:
 * @poppler_annot: a #PopplerAnnotText
 *
 * Retrieves the state of @poppler_annot.
 *
 * Return value: the state of @poppler_annot. %TRUE if it's open, %FALSE in
 *               other case.
 **/
gboolean poppler_annot_text_get_is_open(PopplerAnnotText *poppler_annot)
{
    AnnotText *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_TEXT(poppler_annot), FALSE);

    annot = static_cast<AnnotText *>(POPPLER_ANNOT(poppler_annot)->annot);

    return annot->getOpen();
}

/**
 * poppler_annot_text_set_is_open:
 * @poppler_annot: a #PopplerAnnotText
 * @is_open: whether annotation should initially be displayed open
 *
 * Sets whether @poppler_annot should initially be displayed open
 *
 * Since: 0.16
 */
void poppler_annot_text_set_is_open(PopplerAnnotText *poppler_annot, gboolean is_open)
{
    AnnotText *annot;

    g_return_if_fail(POPPLER_IS_ANNOT_TEXT(poppler_annot));

    annot = static_cast<AnnotText *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot->setOpen(is_open);
}

/**
 * poppler_annot_text_get_icon:
 * @poppler_annot: a #PopplerAnnotText
 *
 * Gets name of the icon of @poppler_annot.
 *
 * Return value: a new allocated string containing the icon name
 */
gchar *poppler_annot_text_get_icon(PopplerAnnotText *poppler_annot)
{
    AnnotText *annot;
    const GooString *text;

    g_return_val_if_fail(POPPLER_IS_ANNOT_TEXT(poppler_annot), NULL);

    annot = static_cast<AnnotText *>(POPPLER_ANNOT(poppler_annot)->annot);

    text = annot->getIcon();

    return text ? _poppler_goo_string_to_utf8(text) : nullptr;
}

/**
 * poppler_annot_text_set_icon:
 * @poppler_annot: a #PopplerAnnotText
 * @icon: the name of an icon
 *
 * Sets the icon of @poppler_annot. The following predefined
 * icons are currently supported:
 * <variablelist>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_NOTE</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_COMMENT</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_KEY</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_HELP</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_NEW_PARAGRAPH</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_PARAGRAPH</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_INSERT</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_CROSS</term>
 *  </varlistentry>
 *  <varlistentry>
 *   <term>#POPPLER_ANNOT_TEXT_ICON_CIRCLE</term>
 *  </varlistentry>
 * </variablelist>
 *
 * Since: 0.16
 */
void poppler_annot_text_set_icon(PopplerAnnotText *poppler_annot, const gchar *icon)
{
    AnnotText *annot;
    GooString *text;

    g_return_if_fail(POPPLER_IS_ANNOT_TEXT(poppler_annot));

    annot = static_cast<AnnotText *>(POPPLER_ANNOT(poppler_annot)->annot);

    text = new GooString(icon);
    annot->setIcon(text);
    delete text;
}

/**
 * poppler_annot_text_get_state:
 * @poppler_annot: a #PopplerAnnotText
 *
 * Retrieves the state of @poppler_annot.
 *
 * Return value: #PopplerAnnotTextState of @poppler_annot.
 **/
PopplerAnnotTextState poppler_annot_text_get_state(PopplerAnnotText *poppler_annot)
{
    AnnotText *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_TEXT(poppler_annot), POPPLER_ANNOT_TEXT_STATE_UNKNOWN);

    annot = static_cast<AnnotText *>(POPPLER_ANNOT(poppler_annot)->annot);

    switch (annot->getState()) {
    case AnnotText::stateUnknown:
        return POPPLER_ANNOT_TEXT_STATE_UNKNOWN;
    case AnnotText::stateMarked:
        return POPPLER_ANNOT_TEXT_STATE_MARKED;
    case AnnotText::stateUnmarked:
        return POPPLER_ANNOT_TEXT_STATE_UNMARKED;
    case AnnotText::stateAccepted:
        return POPPLER_ANNOT_TEXT_STATE_ACCEPTED;
    case AnnotText::stateRejected:
        return POPPLER_ANNOT_TEXT_STATE_REJECTED;
    case AnnotText::stateCancelled:
        return POPPLER_ANNOT_TEXT_STATE_CANCELLED;
    case AnnotText::stateCompleted:
        return POPPLER_ANNOT_TEXT_STATE_COMPLETED;
    case AnnotText::stateNone:
        return POPPLER_ANNOT_TEXT_STATE_NONE;
    default:
        g_warning("Unsupported Annot Text State");
    }

    return POPPLER_ANNOT_TEXT_STATE_UNKNOWN;
}

/* PopplerAnnotTextMarkup */
/**
 * poppler_annot_text_markup_set_quadrilaterals:
 * @poppler_annot: A #PopplerAnnotTextMarkup
 * @quadrilaterals: (element-type PopplerQuadrilateral): A #GArray of
 *   #PopplerQuadrilateral<!-- -->s
 *
 * Set the regions (Quadrilaterals) to apply the text markup in @poppler_annot.
 *
 * Since: 0.26
 **/
void poppler_annot_text_markup_set_quadrilaterals(PopplerAnnotTextMarkup *poppler_annot, GArray *quadrilaterals)
{
    AnnotQuadrilaterals *quads, *quads_temp;
    AnnotTextMarkup *annot;
    const PDFRectangle *crop_box;
    Page *page = nullptr;

    g_return_if_fail(POPPLER_IS_ANNOT_TEXT_MARKUP(poppler_annot));
    g_return_if_fail(quadrilaterals != nullptr && quadrilaterals->len > 0);

    annot = static_cast<AnnotTextMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    crop_box = _poppler_annot_get_cropbox_and_page(POPPLER_ANNOT(poppler_annot), &page);
    quads = create_annot_quads_from_poppler_quads(quadrilaterals);

    if (page && SUPPORTED_ROTATION(page->getRotate())) {
        quads_temp = _page_new_quads_unrotated(page, quads);
        delete quads;
        quads = quads_temp;
    }

    if (!ZERO_CROPBOX(crop_box)) {
        quads_temp = quads;
        quads = new_quads_from_offset_cropbox(crop_box, quads, TRUE);
        delete quads_temp;
    }

    annot->setQuadrilaterals(quads);
    delete quads;
}

/**
 * poppler_annot_text_markup_get_quadrilaterals:
 * @poppler_annot: A #PopplerAnnotTextMarkup
 *
 * Returns a #GArray of #PopplerQuadrilateral items that map from a
 * location on @page to a #PopplerAnnotTextMarkup.  This array must be freed
 * when done.
 *
 * Return value: (element-type PopplerQuadrilateral) (transfer full): A #GArray of #PopplerQuadrilateral
 *
 * Since: 0.26
 **/
GArray *poppler_annot_text_markup_get_quadrilaterals(PopplerAnnotTextMarkup *poppler_annot)
{
    const PDFRectangle *crop_box;
    AnnotTextMarkup *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_TEXT_MARKUP(poppler_annot), NULL);

    annot = static_cast<AnnotTextMarkup *>(POPPLER_ANNOT(poppler_annot)->annot);
    crop_box = _poppler_annot_get_cropbox(POPPLER_ANNOT(poppler_annot));
    AnnotQuadrilaterals *quads = annot->getQuadrilaterals();

    return create_poppler_quads_from_annot_quads(quads, crop_box);
}

/* PopplerAnnotFreeText */
/**
 * poppler_annot_free_text_get_quadding:
 * @poppler_annot: a #PopplerAnnotFreeText
 *
 * Retrieves the justification of the text of @poppler_annot.
 *
 * Return value: #PopplerAnnotFreeTextQuadding of @poppler_annot.
 **/
PopplerAnnotFreeTextQuadding poppler_annot_free_text_get_quadding(PopplerAnnotFreeText *poppler_annot)
{
    AnnotFreeText *annot;

    g_return_val_if_fail(POPPLER_IS_ANNOT_FREE_TEXT(poppler_annot), POPPLER_ANNOT_FREE_TEXT_QUADDING_LEFT_JUSTIFIED);

    annot = static_cast<AnnotFreeText *>(POPPLER_ANNOT(poppler_annot)->annot);

    switch (annot->getQuadding()) {
    case AnnotFreeText::quaddingLeftJustified:
        return POPPLER_ANNOT_FREE_TEXT_QUADDING_LEFT_JUSTIFIED;
    case AnnotFreeText::quaddingCentered:
        return POPPLER_ANNOT_FREE_TEXT_QUADDING_CENTERED;
    case AnnotFreeText::quaddingRightJustified:
        return POPPLER_ANNOT_FREE_TEXT_QUADDING_RIGHT_JUSTIFIED;
    default:
        g_warning("Unsupported Annot Free Text Quadding");
    }

    return POPPLER_ANNOT_FREE_TEXT_QUADDING_LEFT_JUSTIFIED;
}

/**
 * poppler_annot_free_text_get_callout_line:
 * @poppler_annot: a #PopplerAnnotFreeText
 *
 * Retrieves a #PopplerAnnotCalloutLine of four or six numbers specifying a callout
 * line attached to the @poppler_annot.
 *
 * Return value: a new allocated #PopplerAnnotCalloutLine if the annot has a callout
 *               line, %NULL in other case. It must be freed with g_free() when
 *               done.
 **/
PopplerAnnotCalloutLine *poppler_annot_free_text_get_callout_line(PopplerAnnotFreeText *poppler_annot)
{
    AnnotFreeText *annot;
    AnnotCalloutLine *line;

    g_return_val_if_fail(POPPLER_IS_ANNOT_FREE_TEXT(poppler_annot), NULL);

    annot = static_cast<AnnotFreeText *>(POPPLER_ANNOT(poppler_annot)->annot);

    if ((line = annot->getCalloutLine())) {
        AnnotCalloutMultiLine *multiline;
        PopplerAnnotCalloutLine *callout = g_new0(PopplerAnnotCalloutLine, 1);

        callout->x1 = line->getX1();
        callout->y1 = line->getY1();
        callout->x2 = line->getX2();
        callout->y2 = line->getY2();

        if ((multiline = dynamic_cast<AnnotCalloutMultiLine *>(line))) {
            callout->multiline = TRUE;
            callout->x3 = multiline->getX3();
            callout->y3 = multiline->getY3();
            return callout;
        }

        callout->multiline = FALSE;
        return callout;
    }

    return nullptr;
}

/* PopplerAnnotFileAttachment */
/**
 * poppler_annot_file_attachment_get_attachment:
 * @poppler_annot: a #PopplerAnnotFileAttachment
 *
 * Creates a #PopplerAttachment for the file of the file attachment annotation @annot.
 * The #PopplerAttachment must be unrefed with g_object_unref by the caller.
 *
 * Return value: (transfer full): @PopplerAttachment
 *
 * Since: 0.14
 **/
PopplerAttachment *poppler_annot_file_attachment_get_attachment(PopplerAnnotFileAttachment *poppler_annot)
{
    AnnotFileAttachment *annot;
    PopplerAttachment *attachment;

    g_return_val_if_fail(POPPLER_IS_ANNOT_FILE_ATTACHMENT(poppler_annot), NULL);

    annot = static_cast<AnnotFileAttachment *>(POPPLER_ANNOT(poppler_annot)->annot);

    FileSpec *file = new FileSpec(annot->getFile());
    attachment = _poppler_attachment_new(file);
    delete file;

    return attachment;
}

/**
 * poppler_annot_file_attachment_get_name:
 * @poppler_annot: a #PopplerAnnotFileAttachment
 *
 * Retrieves the name of @poppler_annot.
 *
 * Return value: a new allocated string with the name of @poppler_annot. It must
 *               be freed with g_free() when done.
 * Since: 0.14
 **/
gchar *poppler_annot_file_attachment_get_name(PopplerAnnotFileAttachment *poppler_annot)
{
    AnnotFileAttachment *annot;
    const GooString *name;

    g_return_val_if_fail(POPPLER_IS_ANNOT_FILE_ATTACHMENT(poppler_annot), NULL);

    annot = static_cast<AnnotFileAttachment *>(POPPLER_ANNOT(poppler_annot)->annot);
    name = annot->getName();

    return name ? _poppler_goo_string_to_utf8(name) : nullptr;
}

/* PopplerAnnotCalloutLine */
POPPLER_DEFINE_BOXED_TYPE(PopplerAnnotCalloutLine, poppler_annot_callout_line, poppler_annot_callout_line_copy, poppler_annot_callout_line_free)

/**
 * poppler_annot_callout_line_new:
 *
 * Creates a new empty #PopplerAnnotCalloutLine.
 *
 * Return value: a new allocated #PopplerAnnotCalloutLine, %NULL in other case.
 *               It must be freed when done.
 **/
PopplerAnnotCalloutLine *poppler_annot_callout_line_new(void)
{
    return g_new0(PopplerAnnotCalloutLine, 1);
}

/**
 * poppler_annot_callout_line_copy:
 * @callout: the #PopplerAnnotCalloutLine to be copied.
 *
 * It does copy @callout to a new #PopplerAnnotCalloutLine.
 *
 * Return value: a new allocated #PopplerAnnotCalloutLine as exact copy of
 *               @callout, %NULL in other case. It must be freed when done.
 **/
PopplerAnnotCalloutLine *poppler_annot_callout_line_copy(PopplerAnnotCalloutLine *callout)
{
    PopplerAnnotCalloutLine *new_callout;

    g_return_val_if_fail(callout != nullptr, NULL);

    new_callout = g_new0(PopplerAnnotCalloutLine, 1);
    *new_callout = *callout;

    return new_callout;
}

/**
 * poppler_annot_callout_line_free:
 * @callout: a #PopplerAnnotCalloutLine
 *
 * Frees the memory used by #PopplerAnnotCalloutLine.
 **/
void poppler_annot_callout_line_free(PopplerAnnotCalloutLine *callout)
{
    g_free(callout);
}

/* PopplerAnnotMovie */
/**
 * poppler_annot_movie_get_title:
 * @poppler_annot: a #PopplerAnnotMovie
 *
 * Retrieves the movie title of @poppler_annot.
 *
 * Return value: the title text of @poppler_annot.
 *
 * Since: 0.14
 */
gchar *poppler_annot_movie_get_title(PopplerAnnotMovie *poppler_annot)
{
    AnnotMovie *annot;
    const GooString *title;

    g_return_val_if_fail(POPPLER_IS_ANNOT_MOVIE(poppler_annot), NULL);

    annot = static_cast<AnnotMovie *>(POPPLER_ANNOT(poppler_annot)->annot);

    title = annot->getTitle();

    return title ? _poppler_goo_string_to_utf8(title) : nullptr;
}

/**
 * poppler_annot_movie_get_movie:
 * @poppler_annot: a #PopplerAnnotMovie
 *
 * Retrieves the movie object (PopplerMovie) stored in the @poppler_annot.
 *
 * Return value: (transfer none): the movie object stored in the @poppler_annot. The returned
 *               object is owned by #PopplerAnnotMovie and should not be freed
 *
 * Since: 0.14
 */
PopplerMovie *poppler_annot_movie_get_movie(PopplerAnnotMovie *poppler_annot)
{
    return poppler_annot->movie;
}

/* PopplerAnnotScreen */
/**
 * poppler_annot_screen_get_action:
 * @poppler_annot: a #PopplerAnnotScreen
 *
 * Retrieves the action (#PopplerAction) that shall be performed when @poppler_annot is activated
 *
 * Return value: (transfer none): the action to perform. The returned
 *               object is owned by @poppler_annot and should not be freed
 *
 * Since: 0.14
 */
PopplerAction *poppler_annot_screen_get_action(PopplerAnnotScreen *poppler_annot)
{
    return poppler_annot->action;
}

/* PopplerAnnotLine */
/**
 * poppler_annot_line_set_vertices:
 * @poppler_annot: a #PopplerAnnotLine
 * @start: a #PopplerPoint of the starting vertice
 * @end: a #PopplerPoint of the ending vertice
 *
 * Set the coordinate points where the @poppler_annot starts and ends.
 *
 * Since: 0.26
 */
void poppler_annot_line_set_vertices(PopplerAnnotLine *poppler_annot, PopplerPoint *start, PopplerPoint *end)
{
    AnnotLine *annot;

    g_return_if_fail(POPPLER_IS_ANNOT_LINE(poppler_annot));
    g_return_if_fail(start != nullptr);
    g_return_if_fail(end != nullptr);

    annot = static_cast<AnnotLine *>(POPPLER_ANNOT(poppler_annot)->annot);
    annot->setVertices(start->x, start->y, end->x, end->y);
}

/* PopplerAnnotCircle and PopplerAnnotSquare helpers */
static PopplerColor *poppler_annot_geometry_get_interior_color(PopplerAnnot *poppler_annot)
{
    AnnotGeometry *annot;

    annot = static_cast<AnnotGeometry *>(POPPLER_ANNOT(poppler_annot)->annot);

    return create_poppler_color_from_annot_color(annot->getInteriorColor());
}

static void poppler_annot_geometry_set_interior_color(PopplerAnnot *poppler_annot, PopplerColor *poppler_color)
{
    AnnotGeometry *annot;

    annot = static_cast<AnnotGeometry *>(POPPLER_ANNOT(poppler_annot)->annot);

    annot->setInteriorColor(create_annot_color_from_poppler_color(poppler_color));
}

/* PopplerAnnotCircle */
/**
 * poppler_annot_circle_get_interior_color:
 * @poppler_annot: a #PopplerAnnotCircle
 *
 * Retrieves the interior color of @poppler_annot.
 *
 * Return value: a new allocated #PopplerColor with the color values of
 *               @poppler_annot, or %NULL. It must be freed with g_free() when done.
 *
 * Since: 0.26
 */
PopplerColor *poppler_annot_circle_get_interior_color(PopplerAnnotCircle *poppler_annot)
{
    g_return_val_if_fail(POPPLER_IS_ANNOT_CIRCLE(poppler_annot), NULL);

    return poppler_annot_geometry_get_interior_color(POPPLER_ANNOT(poppler_annot));
}

/**
 * poppler_annot_circle_set_interior_color:
 * @poppler_annot: a #PopplerAnnotCircle
 * @poppler_color: (allow-none): a #PopplerColor, or %NULL
 *
 * Sets the interior color of @poppler_annot.
 *
 * Since: 0.26
 */
void poppler_annot_circle_set_interior_color(PopplerAnnotCircle *poppler_annot, PopplerColor *poppler_color)
{
    g_return_if_fail(POPPLER_IS_ANNOT_CIRCLE(poppler_annot));

    poppler_annot_geometry_set_interior_color(POPPLER_ANNOT(poppler_annot), poppler_color);
}

/* PopplerAnnotSquare */
/**
 * poppler_annot_square_get_interior_color:
 * @poppler_annot: a #PopplerAnnotSquare
 *
 * Retrieves the interior color of @poppler_annot.
 *
 * Return value: a new allocated #PopplerColor with the color values of
 *               @poppler_annot, or %NULL. It must be freed with g_free() when done.
 *
 * Since: 0.26
 */
PopplerColor *poppler_annot_square_get_interior_color(PopplerAnnotSquare *poppler_annot)
{
    g_return_val_if_fail(POPPLER_IS_ANNOT_SQUARE(poppler_annot), NULL);

    return poppler_annot_geometry_get_interior_color(POPPLER_ANNOT(poppler_annot));
}

/**
 * poppler_annot_square_set_interior_color:
 * @poppler_annot: a #PopplerAnnotSquare
 * @poppler_color: (allow-none): a #PopplerColor, or %NULL
 *
 * Sets the interior color of @poppler_annot.
 *
 * Since: 0.26
 */
void poppler_annot_square_set_interior_color(PopplerAnnotSquare *poppler_annot, PopplerColor *poppler_color)
{
    g_return_if_fail(POPPLER_IS_ANNOT_SQUARE(poppler_annot));

    poppler_annot_geometry_set_interior_color(POPPLER_ANNOT(poppler_annot), poppler_color);
}
