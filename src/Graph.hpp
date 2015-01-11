//=============================================================================
//File Name: Graph.hpp
//Description: Draws a graph in WinGDI
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Settings.hpp"
#include <list>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <cstdint>

#include <QObject>
#include <QColor>
#include <QTcpSocket>
#include <QHostAddress>

typedef std::pair<float , float> Pair;
typedef std::list<Pair> DataSet;

/* Graph Packet Structure
 *
 * Sending packets:
 *     id can be one of three values with its respective packet structure:
 *
 *     'c': Asks host to start sending data set of given name
 *     struct packet_t {
 *         char id;
 *         char graphName[15];
 *     }
 *
 *     'd': Asks host to stop sending data set of given name
 *     struct packet_t {
 *         char id;
 *         char graphName[15];
 *     }
 *
 *     'l': Asks host to send list of names of available data sets
 *     struct packet_t {
 *         char id;
 *     }
 *
 * Receiving packets:
 *     id can be one of two values with its respective packet structure:
 *
 *     'l': Contains name of data set on host
 *     struct packet_t {
 *         char id;
 *         char graphName[15];
 *     }
 *
 *     'p': Contains point of data from given data set
 *     struct packet_t {
 *         char id;
 *         char graphName[15];
 *         float x;
 *         float y;
 *     }
 */
struct [[gnu::packed]] packet_t {
    char id;
    char graphName[15];
    float x;
    float y;
};

struct [[gnu::packed]] packet_list_t {
    char id;
    char graphName[15];
    char eof;
    char padding[7];
};

class MainWindow;
class SelectDialog;

class Graph : public QObject {
    Q_OBJECT
public:
    explicit Graph( MainWindow* parentWindow );
    virtual ~Graph();

    // Kills receiving thread and restarts it; this function will block
    void reconnect();

    // Add data point to graph at given index (push back)
    void addData( unsigned int index , const Pair& point );

    // Removes all previous data from all graphs
    void clearAllData();

    // Create another set of data to graph
    void createGraph( const std::string& name , QColor color );

    // Remove graph at the given index
    void removeGraph( unsigned int index );

public slots:
    /* Saves all graph data to CSV in the executable's directory
     * returns true upon success
     */
    bool saveAsCSV();

signals:
    void realtimeDataSignal( int graphId , float x , float y );
    void infoDialogSignal( const QString& , const QString& );
    void criticalDialogSignal( const QString& , const QString& );

private slots:
    void handleSocketData();
    void sendGraphChoices();
    void handleStateChange( QAbstractSocket::SocketState state );

private:
    MainWindow* m_window;

    Settings m_settings;

    // Contains graph data to plot
    std::vector<DataSet> m_dataSets;

    std::mutex m_dataMutex;

    // Contains names for all graphs available on host
    std::vector<std::string> m_graphNames;

    // Each bit holds receive state of data set (1 = recv, 0 = not recv)
    uint64_t m_curSelect;

    // Provides way to get a data set's index given the name
    std::map<std::string , unsigned char> m_graphNamesMap;

    enum Error {
        FailConnect = 0,
        Disconnected
    };

    QTcpSocket m_dataSocket;

    QHostAddress m_remoteIP;
    unsigned short m_dataPort;

    char m_buffer[24];
    struct packet_t m_recvData;
    struct packet_list_t m_listData;

    friend class SelectDialog;
};

#endif // GRAPH_HPP
