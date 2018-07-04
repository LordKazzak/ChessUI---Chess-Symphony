#include "board2d.h"

#include <QPainter>
#include <QPaintEvent>
#include <QEvent>
#include <iostream>

#include "piece.h"

Board2D::Board2D(Settings* settings, QWidget *parent) :
    QWidget(parent), Position()
{
    setObjectName("Board2D");
    fliped = false;
    this->settings = settings;
    wSqColor = settings->getWhiteSquareColor();
    bSqColor = settings->getBlackSquareColor();
    font = settings->getBoardFont();
    piecePath = settings->getPiecePath();
    if(!QFile::exists(piecePath)) piecePath = "media";
    wSqPath = settings->getWSqPath();
    if(!QFile::exists(wSqPath)) wSqPath = "media/wsq.png";
    bSqPath = settings->getBSqPath();
    if(!QFile::exists(bSqPath)) bSqPath = "media/bsq.png";
    sWSqColor = settings->getSWSqColor();
    sBSqColor = settings->getSBSqColor();
    king_attacked = NO_SQ;
    color_king = false;
    load();
    from = NO_SQ;
    getMoves(move);

    if(!settings->getIsHuman(stm)){ //če je prvi računalnik
        Engine::instance(); //da se premik doda v position string
        Square tmp = E2;
        drawMove(tmp,E4,stm); //najbolj pogost premik
    }
}

void Board2D::paintEvent(QPaintEvent* e){
    //qDebug() << "PaintEvent entered";
    Piece pt;
    Square sq;
    const int size = std::min((width()/9),(height()/9));
    QPen pen;
    QRect sqRect;
    QPainter p(this);
    pen.setWidth(1);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    QPixmap wSq(QSize(size,size));
    QPixmap bSq(QSize(size,size));
    wSq.fill(wSqColor);
    bSq.fill(bSqColor);
    p.setFont(font);

    for(int i=0; i<8; i++){
        sqRect.setSize(QSize(size/2,size));
        sqRect.moveTo(0,size*i+size/2);
        if(sqRect.intersects(e->rect()))
            p.drawText(sqRect, Qt::AlignCenter, QString('8'-i));
        if(i==0){
            sqRect.moveTo(8*size+2*size/3,size*i+2*size/3);
            if(sqRect.intersects(e->rect())){
                sqRect.setSize(QSize(size/2,size/2));
                if(getStm() == Black){
                    if(sBSqColor) p.fillRect(sqRect,bSq);
                    else p.drawPixmap(sqRect,piece[B]);
                    p.drawRect(sqRect);
                }
                else p.fillRect(sqRect,palette().background());
            }
        }
        else if(i==7){
            sqRect.moveTo(8*size+2*size/3,size*i+2*size/3);
            if(sqRect.intersects(e->rect())){
                sqRect.setSize(QSize(size/2,size/2));
                if(getStm() == White){
                    if(sWSqColor) p.fillRect(sqRect,wSq);
                    else p.drawPixmap(sqRect,piece[W]);
                    p.drawRect(sqRect);
                }
                else p.fillRect(sqRect,palette().background());
            }
        }
        sqRect.setSize(QSize(size,size/2));
        sqRect.moveTo(size/2+size*i,8*size+size/2);
        if(sqRect.intersects(e->rect()))
            p.drawText(sqRect, Qt::AlignCenter, QString('A'+i));
        sqRect.setSize(QSize(size,size));
        for(int j=0; j<8; j++){
            sqRect.moveTo(size/2+(j*size),size/2+(i*size));
            if(!sqRect.intersects(e->rect())) continue;
            if(((i+j) % 2) == 0){
                if(sWSqColor) p.drawPixmap(sqRect, wSq);
                else p.drawPixmap(sqRect,piece[W]);
            }else{
                if(sBSqColor) p.drawPixmap(sqRect, bSq);
                else p.drawPixmap(sqRect,piece[B]);
            }

            if(!fliped){
                sq = Square((7-i)*8+j);
                pt = Position::getPiece(sq);
            }else{
                sq = Square(i*8+(7-j));
                pt = Position::getPiece(sq);
            }

            for(int i=0; i<attacked.count(); i++){ //highlight possible moves for piece
                if(sq == attacked[i]){
                    //qDebug() << "highlited" << QString::fromStdString(square_to_string(sq));
                    p.fillRect(sqRect,palette().highlight());
                    break;
                }
            }

            if(color_king){ //color when check
                if(sq == king_attacked)
                    p.fillRect(sqRect,Qt::red);
            }

            if(sq != from && pt != Empty){
                p.drawPixmap(sqRect,piece[pt]); //draw pieces
            }
            p.drawRect(sqRect);
        }
    }
    if(from != NO_SQ) p.drawPixmap(QRect(mX,mY,size,size),piece[mP]);
}

void Board2D::changeEvent(QEvent * event){
    if(event->type() == QEvent::PaletteChange){
        wSqColor = settings->getWhiteSquareColor();
        bSqColor = settings->getBlackSquareColor();
        font = settings->getBoardFont();
        piecePath = settings->getPiecePath();
        wSqPath = settings->getWSqPath();
        bSqPath = settings->getBSqPath();
        sWSqColor = settings->getSWSqColor();
        sBSqColor = settings->getSBSqColor();
        load();
    }
}

Square Board2D::getSquare(QPoint point){
    int size = std::min((width()/9),(height()/9));
    float x = (point.x()-size/2)/float(size);
    float y = (point.y()-size/2)/float(size);
    if(x < 0 || y < 0 || x > 8 || y > 8) return NO_SQ;
    if(!fliped) return Square((7-int(y))*8+int(x));
    else return Square(int(y)*8+(7-int(x)));
}

void Board2D::mousePressEvent(QMouseEvent* e){
    Qt::MouseButton button = e->button();
    if(button == Qt::LeftButton){
        int i=0, size = std::min((width()/9),(height()/9));
        Square sq = getSquare(e->pos());
        //qDebug() << ("Move from before while: " + QString::fromStdString(square_to_string(sq)));
        for(i = 0; move[i].getType() != NullMove; i++)
            if(move[i].getFrom() == sq)
                break;
        //qDebug() << "Move_type for " << i-1 << move[i-1].getType();
        if(move[i].getType() == NullMove) return;
        from = sq;
        //qDebug() << ("Move from after while: " + QString::fromStdString(square_to_string(from)));
        updateSquare(from,false);
        mX = e->pos().x()-size/2;
        mY = e->pos().y()-size/2;
        mP = getPiece(from);
        QList<Square> attacks = getAttacks(from);

        for(int i=1; i<attacks.count(); i++){ //index 0 is "from" location
            attacked.append(attacks[i]);
            updateSquare(attacks[i],false);
        }

        update(QRect(mX,mY,size,size));
    }
}

void Board2D::mouseMoveEvent(QMouseEvent* e){
    if(from != NO_SQ){
        int size = std::min((width()/9),(height()/9));
        update(QRect(mX,mY,size,size));
        mX = e->pos().x()-size/2;
        mY = e->pos().y()-size/2;
        update(QRect(mX,mY,size,size));
    }
}

void Board2D::mouseReleaseEvent(QMouseEvent* e){
    Color player = stm;

    drawMove(from, getSquare(e->pos()), player);

    for(int i=0; i<attacked.count(); i++){
        updateSquare(attacked[i],false);
        //qDebug() << "attacked: " << QString::fromStdString(square_to_string(attacked[i]));
    }
    //qDebug() << "Player: " << player << "\tStm: " << stm;


    if(color_king && !nullMove/* && !settings->getIsHuman(player)*/){
            color_king = false;
            updateSquare(king_attacked,false);
            //qDebug() << "King Attacked uncolor: " << QString::fromStdString(square_to_string(king_attacked));
            king_attacked = NO_SQ;
        }
    else if(NO_SQ != is_king_attacked(attacked) /*&& NO_SQ == from/*&& !settings->getIsHuman(player) && */){ //if previous move was oponent || nullmove

        color_king = true;
        //if(NO_SQ == king_attacked || !settings->getIsHuman(player))
        king_attacked = is_king_attacked(attacked);

        //qDebug() << "King Attacked color: " << QString::fromStdString(square_to_string(king_attacked));
        updateSquare(king_attacked,false);
    }

    //qDebug() << "Color_king: " << color_king << "\tNullMove: " << nullMove;
    //qDebug() << "King Attacked: " << QString::fromStdString(square_to_string(king_attacked));

    attacked.clear();

    if(move[0].getType() == NullMove){ //game over
        if(stm == White)
            sendTextUI(tr("Mate. Black wins!"));
        else
            sendTextUI(tr("Mate. White wins!"));
    }
}

void Board2D::drawMove(Square &from, Square to, Color &player)
{
    //qDebug() << "draw: " << QString::fromStdString(square_to_string(from)) << QString::fromStdString(square_to_string(to));
    if(from != NO_SQ){
        int i=0;
        int size = std::min((width()/9),(height()/9));
        while(move[i].getType() != NullMove){
            if(move[i].getFrom() == from && move[i].getTo() == to){
                QString tmp = QString::fromStdString(square_to_string(from)) + QString::fromStdString(square_to_string(to));
                //updateSquare(from);
                //updateSquare(to);
                from = NO_SQ;
                nullMove = false;

                moveDo(move[i]);
                if(Engine::engine){
                    std::string tmp = " " + square_to_string(move[i].getFrom()) + square_to_string((move[i].getTo()));
                    Engine::engine->setBoardPosition(tmp); //string with moves
                    //qDebug() << "Position: " << QString::fromStdString(Engine::engine->getPosition());
                }

                update(QRect(mX,mY,size,size));
                updateSquare(to,true);
                //qDebug() << move[i].get_castle();

                //move rook on UI when castling
                if(WK_CASTLE==move[i].get_castle()){
                    updateSquare(F1,true);
                    updateSquare(H1,true);
                    qDebug() << "WK_CASTLE";
                }
                else if(WQ_CASTLE==move[i].get_castle()){
                    updateSquare(A1,true);
                    updateSquare(D1,true);
                    qDebug() << "WQ_CASTLE";
                }
                else if(BK_CASTLE==move[i].get_castle()){
                    updateSquare(F8,true);
                    updateSquare(H8,true);
                    qDebug() << "BK_CASTLE";
                }
                else if(BQ_CASTLE==move[i].get_castle()){
                    updateSquare(A8,true);
                    updateSquare(D8,true);
                    qDebug() << "BQ_CASTLE";
                }
                move[i].set_castle(N0_CASTLE);

                if(enPassant){
                    enPassant = false;
                    if(Black == stm)
                        updateSquare(move[i].getTo()-8,true);
                    else
                        updateSquare(move[i].getTo()+8,true);
                }

                update(QRect(8*size+2*size/3,size*7+2*size/3,size,size));
                update(QRect(8*size+2*size/3,size*0+2*size/3,size,size));
                getMoves(move);

                //show moves on UI
                if(!White == stm)
                    tmp = "White: " + tmp;
                else
                    tmp = "Black: " + tmp;
                sendTextUI(tmp);

                //qDebug() << "Move: " + tmp;

                if(!settings->getIsHuman(stm)) //če je na vrsti računalnik
                {
                    Engine *engine = Engine::instance();
                    QList<Square> squares = engine->move(settings->getDepth(), settings->getTime());
                    //qDebug() << "From: " << QString::fromStdString(square_to_string(squares.at(0))) << "\tTo: " << QString::fromStdString(square_to_string(squares.at(1)));

                    if(squares.at(0) == NO_SQ){
                        qDebug() << "Error: No move";
                        return;
                    }

                    from = squares.at(0);
                    to = squares.at(1);
                    //qDebug() << "in1" << QString::fromStdString(square_to_string(from));
                    drawMove(from, to, player);
                    //qDebug() << "in2" << QString::fromStdString(square_to_string(from));
                    updateSquare(squares.at(0),false);
                }
                //qDebug() << "DrawMove complete";
                //qDebug() << "Move: " + tmp;

                //player = opp_color(stm); //ko je premik veljaven ... za izbris statusa "šaha" na šahovnici
                return;
            }
            i++;
        }
        if(move[i].getType() == NullMove){
            //Square sq = from;
            //from = NO_SQ;
            update(QRect(mX,mY,size,size));
            updateSquare(from, false);
            from = NO_SQ;
            nullMove = true;
        }
        else
            player = opp_color(stm); //ko je premik veljaven ... za izbris statusa "šaha" na šahovnici
    }
}

void Board2D::updateSquare(const Square sq, bool imediate){
    int x,y;
    int size = std::min((width()/9),(height()/9));
    if(fliped){
        x = size/2 + (7-file(sq))*size;
        y = size/2 + rank(sq)*size;
    }
    else{
        x = size/2 + file(sq)*size;
        y = size/2 + (7-rank(sq))*size;
    }
    if(!imediate)
        update(QRect(x,y,size,size));
    else
        repaint(QRect(x,y,size,size)); //during mouserelease when engine is running
}

void Board2D::load(){
    piece[W].load(wSqPath);
    piece[B].load(bSqPath);
    piece[WP].load(piecePath+"/wp.png");
    piece[BP].load(piecePath+"/bp.png");
    piece[WN].load(piecePath+"/wn.png");
    piece[BN].load(piecePath+"/bn.png");
    piece[WB].load(piecePath+"/wb.png");
    piece[BB].load(piecePath+"/bb.png");
    piece[WR].load(piecePath+"/wr.png");
    piece[BR].load(piecePath+"/br.png");
    piece[WQ].load(piecePath+"/wq.png");
    piece[BQ].load(piecePath+"/bq.png");
    piece[WK].load(piecePath+"/wk.png");
    piece[BK].load(piecePath+"/bk.png");
}
