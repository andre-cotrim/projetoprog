//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>
using namespace std;

namespace svg {

    //! A classe "SVGElement" corresponde à classe "mãe" das outras classes posteriormente definidas
    class SVGElement {
        public:
            //! Função construtora "SVGElement" 
            //! Função destrutora "~SVGElement" 
            //! Função "draw" desenha os elementos da imagem
            //! Função "translate" aplica uma translação por um ponto
            //! Função "rotate" aplica uma rotação do elemento por um ângulo em torno de um determinado ponto
            //! Função "scale" aplica a escala desejada no elemento
            //! Função "duplicate" duplica o objeto 
            //! Criámos funções virtuais para poderem ser implementadas nas subclasses
            SVGElement();
            virtual ~SVGElement();
            virtual void draw(PNGImage &img) const = 0;
            virtual void translate(const Point &t) = 0;
            virtual void rotate(Point &origin, int angle) = 0;
            virtual void scale(Point &origin, int v) = 0;
            virtual SVGElement* duplicate() = 0; 
    };

    void readSVG(const std::string &svg_file, Point &dimensions, std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file, const std::string &png_file);

    //! A classe "Ellipse" é uma subclasse de "SVGElement" que define a "Ellipse" através do preenchimento, do centro e dos raios horizontal e vertical
    class Ellipse : public SVGElement {
        public:
            Ellipse(const Color &fill, const Point &center, const Point &radius);
            void draw(PNGImage &img) const;
            void translate(const Point &t); 
            void rotate(Point &origin, int angle);
            void scale(Point &origin, int v);
            Ellipse* duplicate();
        protected:
            Color fill;
            Point center;
            Point radius;
    };

    //! A classe "Circle" é uma subclasse de "Ellipse" que define o "Circle" através do preenchimento, do centro e do raio horizontal e vertical, adquiridos da classe "Ellipse"
    //! Adquire, também, as suas funções através da classe "Ellipse"
    class Circle : public Ellipse {
        public:
            Circle(const Color &fill, const Point &center, const Point &radius);
    };

    //! A classe "Polyline" é uma subclasse de "SVGElement" que define uma "Polyline" através da cor das linhas e de um vetor de pontos
    class Polyline : public SVGElement {
        public:
            Polyline(const Color &stroke, const vector<Point> &points);
            void draw(PNGImage &img) const; 
            void translate(const Point &t);
            void rotate(Point &origin, int angle);
            void scale(Point &origin, int v); 
            Polyline* duplicate();
        protected: 
            Color stroke;
            vector <Point> points;
    };

    //! A classe "Line" é uma subclasse de "Polyline" que define uma "Line" através da cor das linhas, adquirida da classe "Polyline", e de dois pontos
    //! Adquire, também, as suas funções através da classe "Polyline"
    class Line : public Polyline {
        public:
            Line(const Color &stroke, const Point &p1, const Point &p2);
        private:
            Point p1;
            Point p2;
    };

    //! A classe "Polygon" é uma subclasse de "SVGElement" que define um "Polygon" através do preenchimento e de um vetor de pontos
    class Polygon : public SVGElement {
        public:
            Polygon(const Color &fill, const vector<Point> &points);
            void draw(PNGImage &img) const;
            void translate(const Point &t);
            void rotate(Point &origin, int angle);
            void scale(Point &origin, int v); 
            Polygon* duplicate();
        protected:
            Color fill;
            vector<Point> points;
    }; 

    //! A classe "Rect" é uma subclasse de "Polygon" que define um "Rect" através da cor do preenchimento e de um vetor de pontos, adquiridos da classe "Polygon"
    //! É definido através da sua altura e largura para posteriormente calcular os restantes 3 pontos do "Rect", visto que nos ficheiros svg, apenas nos indicam o ponto superior esquerdo
    //! Adquire, também, as suas funções através da classe "Polygon"
    class Rect : public Polygon {
        public:
            Rect(const Color &fill, vector<Point> &points, int width, int height); 
        private:
            int width;
            int height;
    };

    //! A classe "Groups" é uma subclasse de "SVGElement" que define os "Groups" através do vetor de outros elementos
    class Group : public SVGElement {
        public:
            Group(vector<SVGElement*> elements); 
            ~Group();
            void draw(PNGImage &img) const override; 
            void translate(const Point &t);
            void rotate(Point &origin, int angle);
            void scale(Point &origin, int v); 
            Group* duplicate();
        private:
            vector<SVGElement*> elements;
    }; 
}

#endif