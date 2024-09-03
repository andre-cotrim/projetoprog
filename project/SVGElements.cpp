#include "SVGElements.hpp"
#include <vector>
#include <iostream> //
namespace svg
{
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // -----------------------------------------------------------ELLIPSE/CIRCLE--------------------------------------------------------------------------------------------------
    
    //! Função "Ellipse" é o construtor dos elementos "Ellipse"
    //! @param fill define a cor de preenchimento do elemento "Ellipse"
    //! @param center define o centro do elemento "Ellipse"
    //! @param radius define os raios (horizontal e vertical) dos elementos "Ellipse" 
    Ellipse::Ellipse(const Color &fill, const Point &center, const Point &radius) : fill(fill), center(center), radius(radius) {}

    //! Para fazer a função "draw" nos elementos "Ellipse", aplicamos a função "draw_ellipse" definida no ficheiro "PNGImage.cpp"
    //! @param img corresponde à imagem onde se vão desenhar os elementos
    void Ellipse::draw(PNGImage &img) const {
        img.draw_ellipse(center, radius, fill);
    }

    //! Para fazer a função "translate" nos elementos "Ellipse", adicionamos as coordenadas do ponto pelo qual vamos fazer a translação ao ponto do centro da "Ellipse"
    //! @param t direção da translação
    void Ellipse::translate(const Point &t) {
        center = center.translate(t);
    }

    //! Para fazer a função "rotate" nos elementos "Ellipse", aplicamos o ângulo no elemento
    //! @param origin origem de rotação
    //! @param angle ângulo de rotação
    void Ellipse::rotate(Point &origin, int angle) {
        center = center.rotate(origin, angle);
    }

    //! Para fazer a função "scale" dos elementos "Ellipse" altera os raios multiplicando pela escala desejada
    //! @param origin origem da escala
    //! @param v dimensão da escala
    void Ellipse::scale(Point &origin, int v) {
        radius.x *= v;
        radius.y *= v;
        center = center.scale(origin, v);
    }

    //! A função "duplicate" duplica o elemento
    //! @return elemento duplicado
    Ellipse* Ellipse::duplicate() {
        Ellipse *e = new Ellipse(fill, center, radius);
        return e;
    }

    //! Função "Circle" é o construtor dos elementos "Circle"
    //! @param fill define a cor do elemento "Circle"
    //! @param center define o centro do elemento "Circle"
    //! @param radius define o raio dos elementos "Circle"
    Circle::Circle(const Color &fill, const Point &center, const Point &radius) : Ellipse(fill, center, radius) {}

    //-------------------------------------------------------POLYLINE/LINE---------------------------------------------------------------------------------------------------
    
    //! Função "Polyline" é o construtor dos elementos "Polyline"
    //! @param stroke define a cor do traço do elemento "Polyline"
    //! @param points é um vetor com todos os pontos do elemento "Polyline"
    Polyline::Polyline(const Color &stroke, const vector<Point> &points) : stroke(stroke), points(points) {} 

    //! Para aplicar a função "draw" nos elementos "Polyline" utilizamos um ciclo "for" que itera sobre um vetor que contém todos os pontos das retas que criam o elemento
    //! Com a iteração criamos dois pontos e desenhamos linha a linha através da função "draw_line" definida em "PNGImage.cpp"
    //! @param img corresponde à imagem onde se vão desenhar os elementos
    void Polyline::draw(PNGImage &img) const {
        Point point1, point2;
        for(long unsigned int i = 0; (i + 1) < points.size(); i++) {
            point1 = points[i];
            point2 = points[i + 1];
            img.draw_line(point1, point2, stroke);
        }
    }

    //! Para aplicar a função "translate" nos elementos "Polyline", utilizamos um ciclo "for" que itera sobre um vetor de pontos, fazendo a transformação ponto a ponto
    //! @param t direção da translação
    void Polyline::translate(const Point &t) { 
        for(Point &p : points) {
            p = p.translate(t);
        }
    }

    //! Para aplicar a função "rotate" nos elementos "Polyline", utilizamos um ciclo "for" que itera sobre um vetor de pontos, fazendo a transformação ponto a ponto
    //! @param origin origem de rotação
    //! @param angle ângulo de rotação 
    void Polyline::rotate(Point &origin, int angle) {
        for(Point &p : points) {
            p = p.rotate(origin, angle);
        }
    }

    //! Para aplicar a função "scale" nos elementos "Polyline", utilizamos um ciclo "for" que itera sobre um vetor de pontos, fazendo a transformação ponto a ponto
    //! @param origin origem da escala
    //! @param v dimensão da escala
    void Polyline::scale(Point &origin, int v) {
        for(Point &p : points) {
            p = p.scale(origin, v);
        }
    } 

    //! A função "duplicate" duplica o elemento
    //! @return elemento duplicado
    Polyline* Polyline::duplicate() {
        Polyline *e = new Polyline(stroke, points);
        return e;
    } 

    //! Função "Line" é o construtor dos elementos "Line"
    //! @param stroke define a cor do traço do elemento "Line"
    //! @param p1 primeiro ponto de "Line"
    //! @param p2 segundo ponto de "Line"
    Line::Line(const Color &stroke, const Point &p1,const Point &p2) : Polyline(stroke, {p1, p2}) {} 

    //-----------------------------------------------------POLYGON--------------------------------------------------------------------------------------------------------

    //! Função "Polygon" é o construtor dos elementos "Polygon"
    //! @param fill define a cor de preenchimento do elemento "Polygon"
    //! @param points é um vetor com todos os pontos do elemento "Polygon"
    Polygon::Polygon(const Color &fill, const vector<Point> &points) : fill(fill), points(points) {}

    //! Para aplicar a função "draw" nos elementos "Polygon" utilizamos a função "draw_polygon" definida no ficheiro "PNGImage.cpp"
    //! @param img corresponde à imagem onde se vão desenhar os elementos
    void Polygon::draw(PNGImage &img) const {
        img.draw_polygon(points, fill);
    }

    //! Para aplicar a função "translate" nos elementos "Polygon", utilizamos um ciclo "for" que itera sobre um vetor de pontos, fazendo a transformação ponto a ponto
    //! @param t direção da translação
    void Polygon::translate(const Point &t) { 
        for(Point &p : points) {
            p = p.translate(t);
        }
    }

    //! Para aplicar a função "rotate" nos elementos "Polygon", utilizamos um ciclo "for" que itera sobre um vetor de pontos, fazendo a transformação ponto a ponto
    //! @param origin origem de rotação
    //! @param angle ângulo de rotação 
    void Polygon::rotate(Point &origin, int angle) {
        for(Point &p : points) {
            p = p.rotate(origin, angle);
        }
    }

    //! Para aplicar a função "scale" nos elementos "Polygon", utilizamos um ciclo "for" que itera sobre um vetor de pontos, fazendo a transformação ponto a ponto
    //! @param origin origem da escala
    //! @param v dimensão da escala
    void Polygon::scale(Point &origin, int v) {
        for(Point &p : points) {
            p = p.scale(origin, v);
        }
    }

    //! A função "duplicate" duplica o elemento
    //! @return elemento duplicado
    Polygon* Polygon::duplicate() {
        Polygon *e = new Polygon(fill, points);
        return e;
    } 

    //--------------------------------------------------------RECT-----------------------------------------------------------------------------------------------------

    //! Função Rect é o construtor dos elementos "Rect"
    //! @param fill define a cor de preenchimento do elemento "Rect"
    //! @param points é o vetor onde colocamos os pontos do elemento
    //! @param width é valor da largura do elemento
    //! @param height é o valor da altura do elemento
    Rect::Rect(const Color &fill, vector<Point> &points, int width, int height) : Polygon(fill,points), width(width), height(height) {} 

    //-------------------------------------------------------GROUPS----------------------------------------------------------------------------------------------------

    //! Função "Group" é o construtor dos elementos "Group"
    //! @param elements é o vetor com todos os elementos do grupo
    Group::Group(vector<SVGElement*> elements) : elements(elements) {}

    //! Função para desenhar o grupo, desenhando todos os elementos no grupo iterativamente
    //! @param img corresponde à imagem onde se vão desenhar os elementos
    void Group::draw(PNGImage &img) const {
        for (SVGElement* element : elements) {
            element -> draw(img);
        }
    }
    
    //! Para aplicar a função "translate" em grupos, utilizamos um ciclo "for" que itera sobre um vetor de elementos, fazendo a transformação elemento a elemento
    //! @param t direção da translação
    void Group::translate(const Point &t) {
        for (SVGElement* element : elements) {
            element -> translate(t);
        }
    }

    //! Para aplicar a função "rotate" em grupos, utilizamos um ciclo "for" que itera sobre um vetor de elementos, fazendo a transformação elemento a elemento
    //! @param origin origem de rotação
    //! @param angle ângulo de rotação 
    void Group::rotate(Point &origin, int angle) {
        for (SVGElement* element : elements) {
            element -> rotate(origin,angle);
        }
    }

    //! Para aplicar a função "scale" em grupos, utilizamos um ciclo "for" que itera sobre um vetor de elementos, fazendo a transformação elemento a elemento
    //! @param origin origem da escala
    //! @param v dimensão da escala
    void Group::scale(Point &origin, int v) {
        for (SVGElement* element : elements) {
            element -> scale(origin, v);
        }
    }

    //! A função "duplicate" duplica o grupo de elemento
    //! @return grupo de elementos duplicado
    Group* Group::duplicate() {
        vector<SVGElement*> g_dup;
        for (SVGElement* element : elements) {
            g_dup.push_back(element -> duplicate());
        }
        Group *e = new Group(g_dup);
        return e;
    }

    //! Função para eliminar do vetor os elementos do grupo
    Group::~Group() {
        for (SVGElement *element : elements) {
            delete element;
        }
    }
}
 
