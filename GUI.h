#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <string>
#include "UiControls.h"
#include "DataManager.h"
#include "AVLTree.h"
#include "Backtracking.h"

enum class Screen {
    DASHBOARD,
    SETUP_DATA,
    TIMETABLE,
    SEARCH_COURSE,
    ADD_COURSE
};

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    Screen currentScreen;

    // Backend & Data Structures
    DataManager manager;
    CourseAVLTree courseTree;
    Backtracking solver;

    // Status Banner & State
    std::string statusBanner;
    sf::Color statusColor;
    bool timetableGenerated;
    int selectedSectionIdx;

    // Top Navigation Bar
    Button btnNavDashboard;
    Button btnNavSetup;
    Button btnNavAdd;
    Button btnNavTimetable;
    Button btnNavSearch;

    // Dashboard Controls
    Button btnGenerate;
    Button btnResetAll;

    // Setup Data Controls (Teachers, Rooms, Sections)
    TextBox txtNewTeacherId;
    TextBox txtNewTeacherName;
    Button btnAddTeacher;

    TextBox txtNewRoomId;
    TextBox txtNewRoomName;
    TextBox txtNewRoomCap;
    Button btnToggleRoomLab;
    Button btnAddRoom;
    bool newRoomIsLab;

    TextBox txtNewSectionId;
    TextBox txtNewSectionName;
    TextBox txtNewSectionSize;
    Button btnAddSection;

    // Timetable View Controls
    Button btnPrevSection;
    Button btnNextSection;
    Button btnRegenerate;

    // Search Course (AVL) Controls
    TextBox txtSearchId;
    Button btnSearchCourse;
    CourseOffering* searchedCourse;
    bool hasSearched;

    // Add Course Controls
    TextBox txtCourseId;
    TextBox txtCourseName;
    TextBox txtCreditHours;
    TextBox txtTeacherId;
    TextBox txtSectionId;
    TextBox txtLecturesPerWeek;
    Button btnToggleLab;
    Button btnSubmitCourse;
    bool newCourseIsLab;

    // Helper Methods
    void rebuildAVLTree();
    std::string getTeacherName(int teacherId);
    std::string getRoomName(int roomId);
    std::string getSectionName(CourseOffering* c);

    // Core Loop & Rendering Methods
    void handleEvents();
    void updateHoverStates(sf::Vector2f mousePos);
    void render();

    void drawTopNav();
    void drawStatusBanner();
    void drawDashboard();
    void drawSetupData();
    void drawTimetable();
    void drawSearchCourse();
    void drawAddCourse();

public:
    GUI();
    void run();
};

#endif // GUI_H