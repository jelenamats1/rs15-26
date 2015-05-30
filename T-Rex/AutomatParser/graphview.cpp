#include <math.h>
#include "graphview.h"
#include <iostream>

#include <iostream>
using namespace std;

GraphView::GraphView()
 :QGraphicsScene()
{

}

GraphView::GraphView(const QString& filename, QWidget* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    renderGraph(filename);
}


GraphView::GraphView(graph_t* graph, QWidget* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    renderGraph(graph);
}

GraphView::~GraphView()
{
    clearGraph();
}


QPointF
GraphView::gToQ(const point& p, bool upside_down) const
{
    return upside_down ? QPointF(p.x, graphRect.height() - p.y) : QPointF(p.x, -p.y);
}

QPointF
GraphView::gToQ(const pointf& p, bool upside_down) const
{
    return upside_down ? QPointF(p.x, graphRect.height() - p.y) : QPointF(p.x, -p.y);
}


QString
GraphView::aggetToQString(void* obj, const char* name, const QString& fallback) const
{
    const char* tmp = agget(obj, const_cast<char*>(name));
    if (tmp == NULL || strlen(tmp) == 0)
    return fallback;
    return QString::fromUtf8(tmp);
}

QPainterPath
GraphView::makeBezier(const bezier& bezier) const
{
    QPainterPath path;
    path.moveTo(gToQ(bezier.list[0]));
    for (int i = 1; i < bezier.size - 1; i += 3)
    path.cubicTo(gToQ(bezier.list[i]), gToQ(bezier.list[i+1]), gToQ(bezier.list[i+2]));
    return path;
}


void
GraphView::drawArrow(const QLineF& line, const textlabel_t* textlabel, const QColor& color, QPainter* painter) const
{
    QLineF n(line.normalVector());
    QPointF o(n.dx() / 3.0, n.dy() / 3.0);

    QPolygonF polygon;
    polygon.append(line.p1() + o);
    polygon.append(line.p2());
    polygon.append(line.p1() - o);

    QPen pen(color);
    pen.setWidthF(1.0);
    painter->setPen(pen);

    QBrush brush(color);
    painter->setBrush(brush);

    painter->drawPolygon(polygon);

    painter->setPen(textlabel->fontcolor);

    QFont font(textlabel->fontname, textlabel->fontsize);
    font.setPixelSize(textlabel->fontsize);
    painter->setFont(font);

    QString text(QString::fromUtf8(textlabel->text));
    QFontMetricsF fm(painter->fontMetrics());
    QRectF rect(fm.boundingRect(text));
    rect.moveCenter(gToQ(textlabel->pos, true));
    painter->drawText(rect.adjusted(-1, -1, +1, +1), Qt::AlignCenter, text);

}


void
GraphView::renderGraph(const QString& filename)
{
    FILE* fp = fopen(filename.toUtf8().constData(), "r");
    if (fp)
    {
    GVC_t* gvc = gvContext();
    if (gvc != NULL)
    {
        graph_t* graph = agread(fp,0);
        if (graph != NULL)
        {
        if (gvLayout(gvc, graph, "dot") == 0)
        {
            renderGraph(graph);
            gvFreeLayout(gvc, graph);
        }
        else
        {
            qCritical("gvLayout() failed");
        }

        agclose(graph);
        }
        else
        {
        qCritical("agread() failed");
        }

        gvFreeContext(gvc);
    }
    else
    {
        qCritical("gvContext() failed");
    }

    fclose(fp);
    }
    else
    {
    qCritical("failed to open %s", filename.toUtf8().constData());
    }
}


void GraphView::make_ellipse_helper(node_t *node, QPainterPath &path) const {
    const polygon_t *const poly = static_cast<polygon_t *>(ND_shape_info(node));

    if(poly->peripheries != 1) {
      //  qWarning("unsupported number of peripheries %d", poly->peripheries);
    }

    const int sides = poly->sides;
    const pointf* vertices = poly->vertices;

    QPolygonF polygon;
    for (int side = 0; side < sides; side++) {
        polygon.append(gToQ(vertices[side], false));
    }

    QRectF ellipse_bounds(polygon[0], polygon[1]);

    for (int i = 0; i < poly->peripheries; ++i) {
        path.addEllipse(ellipse_bounds.adjusted(-2 * i, 2 * i, 2 * i, -2 * i));
    }
}


QPainterPath GraphView::make_shape(node_t *node) const {
    QPainterPath path;

    const QString name = QString::fromUtf8(ND_shape(node)->name);


    if(name == "ellipse" || name=="doublecircle" || name == "circle" || name == "point" || name == "Mcircle") {
        make_ellipse_helper(node, path);
    } else if(name == "none") {
        // NO-OP
    } else {
        qWarning("unsupported shape %s", qPrintable(name));
    }

    return path;
}


void
GraphView::drawLabel(const textlabel_t* textlabel, QPainter* painter) const
{
    painter->setPen(textlabel->fontcolor);

    QFont font(textlabel->fontname, textlabel->fontsize);
    font.setPixelSize(textlabel->fontsize);
    painter->setFont(font);

    QString text(QString::fromUtf8(textlabel->text));
    QFontMetricsF fm(painter->fontMetrics());
    QRectF rect(fm.boundingRect(text));
    rect.moveCenter(gToQ(textlabel->pos, false));
    painter->drawText(rect.adjusted(-1, -1, +1, +1), Qt::AlignCenter, text);
}

void
GraphView::drawLabel2(const textlabel_t* textlabel, QPainter* painter) const
{

}


void
GraphView::clearGraph()
{
    QList<QGraphicsItem*> items(this->items());

    while (!items.isEmpty())
    delete items.takeFirst();
}


void
GraphView::renderGraph(graph_t* graph)
{
    clearGraph();

    graphRect = QRectF(GD_bb(graph).LL.x, GD_bb(graph).LL.y, GD_bb(graph).UR.x, GD_bb(graph).UR.y);
    setSceneRect(graphRect.adjusted(-5, -5, +5, +5));

    setBackgroundBrush(Qt::white);

    for (node_t* node = agfstnode(graph); node != NULL; node = agnxtnode(graph, node))
        {
        QPicture picture;
        QPainter painter;

        painter.begin(&picture);
        drawLabel(ND_label(node), &painter);
        painter.end();

        QString tekst(ND_label(node)->text);
        GraphNode * item;
        if (tekst=="")
            item = new GraphNode(-1, make_shape(node), picture);
        else
            item = new GraphNode(QString(ND_label(node)->text).toInt(), make_shape(node), picture);

        item->setPos(gToQ(ND_coord(node)));

        QPen pen(Qt::black);
        pen.setWidthF(1.0);
        item->setPen(pen);

        QBrush brush(Qt::green);
        item->setBrush(brush);


        QString tooltip = aggetToQString(node, "tooltip", ND_label(node)->text);
        if (!tooltip.isEmpty())
        {
            tooltip.replace("\\n", "\n");
            item->setToolTip(tooltip);
        }

        addItem(item);


        for (edge_t* edge = agfstout(graph, node); edge != NULL; edge = agnxtout(graph, edge))
        {
            const splines* spl = ED_spl(edge);
            if (spl == NULL)
            continue;

            QPicture picture1;
            QPainter painter1;

            if (ED_label(edge)==0)
                continue;
            painter1.begin(&picture1);
          //  drawLabel2(ED_label(edge),&painter1);
            //painter1.end();

            for (int i = 0; i < spl->size; ++i)
            {
            const bezier& bz = spl->list[i];

            QColor color(Qt::black);

            QPainterPath path(makeBezier(bz));
           // QPicture picture;
           // QPainter painter;


             //painter.begin(&picture);
            if (bz.sflag)
                drawArrow(QLineF(gToQ(bz.list[0]), gToQ(bz.sp)),ED_label(edge), color, &painter1);
            if (bz.eflag)
                drawArrow(QLineF(gToQ(bz.list[bz.size-1]), gToQ(bz.ep)),ED_label(edge), color, &painter1);
             painter1.end();

            QString text1(ND_label(node)->text);
            QString text2(ND_label(edge->node)->text);
            int state1;
            int state2;
            if (text1 == "")
              state1 = -1;
            else
              state1 = text1.toInt();
            if (text2 == "")
                state2 = -1;
            else
               state2 = text2.toInt();
            char c = ED_label(edge)->text[0];

            GraphEdge* item = new GraphEdge(state1,state2,c,path, picture1);

            QPen pen(color);
            pen.setStyle(Qt::SolidLine);
            pen.setWidthF(1.0);
            item->setPen(pen);

            item->setZValue(-1.0);

            addItem(item);
            //edges.append(item);
            }
        }
        }
    }

GraphItem::GraphItem(const QPainterPath& path, const QPicture& picture)
    : QGraphicsPathItem(path),
      picture(picture)
{
}



GraphNode::GraphNode(int state, const QPainterPath& path, const QPicture& picture)
    : GraphItem(path,picture),
      state(state)
{
}


void
GraphItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    painter->save();
    QGraphicsPathItem::paint(painter, option, widget);
    painter->restore();
    picture.play(painter);
}


GraphEdge::GraphEdge(int state1, int state2, char c, const QPainterPath& path, const QPicture& picture)
    : GraphItem(path,picture),
      state1(state1),
      state2(state2),
      c(c)
{
}


QRectF
GraphEdge::boundingRect() const
{
    return QGraphicsPathItem::boundingRect().united(picture.boundingRect());
}


