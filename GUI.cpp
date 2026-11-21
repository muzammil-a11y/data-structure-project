#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Helper to load a system font on Windows/MSYS2 cleanly
static sf::Font loadSystemFont() {
    sf::Font f;
    if (f.openFromFile("C:/Windows/Fonts/segoeui.ttf")) return f;
    if (f.openFromFile("C:/Windows/Fonts/arial.ttf")) return f;
    if (f.openFromFile("arial.ttf")) return f;
    std::cerr << "Warning: Could not load system font.\n";
    return f;
}

GUI::GUI()
    : window(sf::VideoMode({1280, 760}), "University Timetable Generator - DS Project"),
      font(loadSystemFont()),
      currentScreen(Screen::DASHBOARD),
      statusBanner("Ready. Add teachers, rooms, sections, and courses to begin."),
      statusColor(sf::Color(40, 140, 90)),
      timetableGenerated(false),
      selectedSectionIdx(-1),

      // Top Navigation Bar
      btnNavDashboard(font, "Dashboard", {30.f, 20.f}, {165.f, 42.f}),
      btnNavSetup(font, "1. Setup Data", {210.f, 20.f}, {175.f, 42.f}),
      btnNavAdd(font, "2. Add Course", {400.f, 20.f}, {175.f, 42.f}),
      btnNavTimetable(font, "View Timetable", {590.f, 20.f}, {185.f, 42.f}),
      btnNavSearch(font, "Search (AVL)", {790.f, 20.f}, {175.f, 42.f}),
      
      // Dashboard Buttons
      btnGenerate(font, "Generate Timetable", {80.f, 230.f}, {280.f, 52.f}, sf::Color(35, 145, 85), sf::Color(55, 175, 110)),
      btnResetAll(font, "Reset Availability", {390.f, 230.f}, {240.f, 52.f}, sf::Color(180, 70, 70), sf::Color(210, 95, 95)),
      
      // Setup Data Controls (Teachers, Rooms, Sections)
      txtNewTeacherId(font, "Teacher ID ", {60.f, 200.f}, {320.f, 42.f}, true, 10),
      txtNewTeacherName(font, "Teacher Name", {60.f, 255.f}, {320.f, 42.f}, false, 30),
      btnAddTeacher(font, "Add Teacher", {60.f, 315.f}, {320.f, 44.f}, sf::Color(45, 110, 210), sf::Color(70, 140, 240)),

      txtNewRoomId(font, "Room ID", {440.f, 200.f}, {320.f, 42.f}, true, 10),
      txtNewRoomName(font, "Room Name", {440.f, 255.f}, {320.f, 42.f}, false, 25),
      txtNewRoomCap(font, "Capacity", {440.f, 310.f}, {320.f, 42.f}, true, 4),
      btnToggleRoomLab(font, "Room Type: Lecture Hall", {440.f, 365.f}, {320.f, 42.f}, sf::Color(90, 100, 120), sf::Color(115, 125, 145)),
      btnAddRoom(font, "Add Room", {440.f, 420.f}, {320.f, 44.f}, sf::Color(45, 110, 210), sf::Color(70, 140, 240)),
      newRoomIsLab(false),

      txtNewSectionId(font, "Section ID", {820.f, 200.f}, {320.f, 42.f}, true, 10),
      txtNewSectionName(font, "Section Name", {820.f, 255.f}, {320.f, 42.f}, false, 20),
      txtNewSectionSize(font, "Student Count", {820.f, 310.f}, {320.f, 42.f}, true, 4),
      btnAddSection(font, "Add Section", {820.f, 365.f}, {320.f, 44.f}, sf::Color(45, 110, 210), sf::Color(70, 140, 240)),

      // Timetable Filter Buttons
      btnPrevSection(font, "< Prev Section", {720.f, 100.f}, {160.f, 38.f}),
      btnNextSection(font, "Next Section >", {900.f, 100.f}, {160.f, 38.f}),
      btnRegenerate(font, "Re-Solve", {1080.f, 100.f}, {150.f, 38.f}, sf::Color(35, 145, 85), sf::Color(55, 175, 110)),
      
      // Search Course Controls
      txtSearchId(font, "Enter Course ID", {80.f, 180.f}, {320.f, 46.f}, true, 10),
      btnSearchCourse(font, "Search", {420.f, 180.f}, {210.f, 46.f}),
      searchedCourse(nullptr),
      hasSearched(false),
      
      // Add Course Controls
      txtCourseId(font, "Course ID ", {80.f, 170.f}, {340.f, 44.f}, true, 10),
      txtCourseName(font, "Course Name", {80.f, 235.f}, {340.f, 44.f}, false, 35),
      txtCreditHours(font, "Credit Hours", {80.f, 300.f}, {340.f, 44.f}, true, 2),
      txtTeacherId(font, "Teacher ID ", {480.f, 170.f}, {340.f, 44.f}, true, 10),
      txtSectionId(font, "Section ID", {480.f, 235.f}, {340.f, 44.f}, true, 10),
      txtLecturesPerWeek(font, "Lectures / Week", {480.f, 300.f}, {340.f, 44.f}, true, 2),
      btnToggleLab(font, "Type: Theory Lecture", {80.f, 370.f}, {340.f, 46.f}, sf::Color(90, 100, 120), sf::Color(115, 125, 145)),
      btnSubmitCourse(font, "Add Course & Re-Link", {480.f, 370.f}, {340.f, 46.f}, sf::Color(35, 145, 85), sf::Color(55, 175, 110)),
      newCourseIsLab(false)
{
    window.setFramerateLimit(60);
}

void GUI::rebuildAVLTree() {
    courseTree.clear();
    CourseArray& courses = manager.getCourses();
    for (int i = 0; i < courses.size(); i++) {
        courseTree.insert(courses.get(i));
    }
}

std::string GUI::getTeacherName(int teacherId) {
    for (int i = 0; i < manager.teachers.size(); i++) {
        if (manager.teachers.get(i).teacherID == teacherId) {
            return manager.teachers.get(i).name;
        }
    }
    return "T-" + std::to_string(teacherId);
}

std::string GUI::getRoomName(int roomId) {
    for (int i = 0; i < manager.rooms.size(); i++) {
        if (manager.rooms.get(i).roomID == roomId) {
            return manager.rooms.get(i).roomName;
        }
    }
    return "R-" + std::to_string(roomId);
}

std::string GUI::getSectionName(CourseOffering* c) {
    if (c != nullptr && c->section != nullptr) {
        return c->section->name;
    }
    return "All";
}

void GUI::run() {
    while (window.isOpen()) {
        handleEvents();
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        updateHoverStates(worldPos);
        render();
    }
}

void GUI::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (currentScreen == Screen::SETUP_DATA) {
                txtNewTeacherId.handleTextEntered(textEntered->unicode);
                txtNewTeacherName.handleTextEntered(textEntered->unicode);
                txtNewRoomId.handleTextEntered(textEntered->unicode);
                txtNewRoomName.handleTextEntered(textEntered->unicode);
                txtNewRoomCap.handleTextEntered(textEntered->unicode);
                txtNewSectionId.handleTextEntered(textEntered->unicode);
                txtNewSectionName.handleTextEntered(textEntered->unicode);
                txtNewSectionSize.handleTextEntered(textEntered->unicode);
            }
            else if (currentScreen == Screen::SEARCH_COURSE) {
                txtSearchId.handleTextEntered(textEntered->unicode);
            }
            else if (currentScreen == Screen::ADD_COURSE) {
                txtCourseId.handleTextEntered(textEntered->unicode);
                txtCourseName.handleTextEntered(textEntered->unicode);
                txtCreditHours.handleTextEntered(textEntered->unicode);
                txtTeacherId.handleTextEntered(textEntered->unicode);
                txtSectionId.handleTextEntered(textEntered->unicode);
                txtLecturesPerWeek.handleTextEntered(textEntered->unicode);
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);

                // Top Navigation Bar Clicks
                if (btnNavDashboard.isClicked(mousePos)) currentScreen = Screen::DASHBOARD;
                else if (btnNavSetup.isClicked(mousePos)) currentScreen = Screen::SETUP_DATA;
                else if (btnNavAdd.isClicked(mousePos)) currentScreen = Screen::ADD_COURSE;
                else if (btnNavTimetable.isClicked(mousePos)) currentScreen = Screen::TIMETABLE;
                else if (btnNavSearch.isClicked(mousePos)) currentScreen = Screen::SEARCH_COURSE;

                // Screen-specific Clicks
                if (currentScreen == Screen::DASHBOARD) {
                    if (btnGenerate.isClicked(mousePos)) {
                        if (manager.getCourses().size() == 0) {
                            statusBanner = "Add at least one course before generating a timetable.";
                            statusColor = sf::Color(190, 60, 60);
                        } else if (manager.rooms.size() == 0 || manager.teachers.size() == 0 || manager.sections.size() == 0) {
                            statusBanner = "Missing rooms, teachers, or sections! Add them in '1. Setup Data' first.";
                            statusColor = sf::Color(190, 60, 60);
                        } else {
                            manager.resetAllAvailability();
                            bool ok = solver.generateTimetable(manager);
                            if (ok) {
                                timetableGenerated = true;
                                selectedSectionIdx = -1;
                                statusBanner = "Timetable generated successfully! (" +
                                               std::to_string(manager.getFinalSchedule().size()) + " scheduled slots)";
                                statusColor = sf::Color(40, 140, 90);
                                currentScreen = Screen::TIMETABLE;
                            } else {
                                timetableGenerated = false;
                                statusBanner = "Backtracking could not find a conflict-free timetable.";
                                statusColor = sf::Color(190, 60, 60);
                            }
                        }
                    }
                    else if (btnResetAll.isClicked(mousePos)) {
                        manager.resetAllAvailability();
                        manager.getFinalSchedule().clear();
                        timetableGenerated = false;
                        statusBanner = "Cleared generated schedule & reset teacher/room/section availability.";
                        statusColor = sf::Color(60, 120, 190);
                    }
                }
                else if (currentScreen == Screen::SETUP_DATA) {
                    txtNewTeacherId.handleClick(mousePos);
                    txtNewTeacherName.handleClick(mousePos);
                    txtNewRoomId.handleClick(mousePos);
                    txtNewRoomName.handleClick(mousePos);
                    txtNewRoomCap.handleClick(mousePos);
                    txtNewSectionId.handleClick(mousePos);
                    txtNewSectionName.handleClick(mousePos);
                    txtNewSectionSize.handleClick(mousePos);

                    if (btnAddTeacher.isClicked(mousePos)) {
                        if (txtNewTeacherId.isEmpty() || txtNewTeacherName.isEmpty()) {
                            statusBanner = "Enter both Teacher ID and Teacher Name.";
                            statusColor = sf::Color(190, 60, 60);
                        } else {
                            int tId = std::stoi(txtNewTeacherId.getText());
                            bool exists = false;
                            for (int i = 0; i < manager.teachers.size(); i++) {
                                if (manager.teachers.get(i).teacherID == tId) exists = true;
                            }
                            if (exists) {
                                statusBanner = "Teacher ID " + std::to_string(tId) + " already exists!";
                                statusColor = sf::Color(190, 60, 60);
                            } else {
                                Teacher t;
                                t.teacherID = tId;
                                t.name = txtNewTeacherName.getText();
                                manager.teachers.add(t);
                                manager.linkData();
                                txtNewTeacherId.clear();
                                txtNewTeacherName.clear();
                                statusBanner = "Added Teacher: " + t.name + " (ID " + std::to_string(tId) + ")";
                                statusColor = sf::Color(40, 140, 90);
                            }
                        }
                    }
                    else if (btnToggleRoomLab.isClicked(mousePos)) {
                        newRoomIsLab = !newRoomIsLab;
                        btnToggleRoomLab.setText(newRoomIsLab ? "Room Type: Lab" : "Room Type: Lecture Hall");
                    }
                    else if (btnAddRoom.isClicked(mousePos)) {
                        if (txtNewRoomId.isEmpty() || txtNewRoomName.isEmpty() || txtNewRoomCap.isEmpty()) {
                            statusBanner = "Enter Room ID, Room Name, and Capacity.";
                            statusColor = sf::Color(190, 60, 60);
                        } else {
                            int rId = std::stoi(txtNewRoomId.getText());
                            bool exists = false;
                            for (int i = 0; i < manager.rooms.size(); i++) {
                                if (manager.rooms.get(i).roomID == rId) exists = true;
                            }
                            if (exists) {
                                statusBanner = "Room ID " + std::to_string(rId) + " already exists!";
                                statusColor = sf::Color(190, 60, 60);
                            } else {
                                Room r;
                                r.roomID = rId;
                                r.roomName = txtNewRoomName.getText();
                                r.capacity = std::stoi(txtNewRoomCap.getText());
                                r.isLab = newRoomIsLab;
                                manager.rooms.add(r);
                                txtNewRoomId.clear();
                                txtNewRoomName.clear();
                                txtNewRoomCap.clear();
                                statusBanner = "Added Room: " + r.roomName + " (ID " + std::to_string(rId) + ")";
                                statusColor = sf::Color(40, 140, 90);
                            }
                        }
                    }
                    else if (btnAddSection.isClicked(mousePos)) {
                        if (txtNewSectionId.isEmpty() || txtNewSectionName.isEmpty()) {
                            statusBanner = "Enter Section ID and Section Name.";
                            statusColor = sf::Color(190, 60, 60);
                        } else {
                            int sId = std::stoi(txtNewSectionId.getText());
                            bool exists = false;
                            for (int i = 0; i < manager.sections.size(); i++) {
                                if (manager.sections.get(i).sectionID == sId) exists = true;
                            }
                            if (exists) {
                                statusBanner = "Section ID " + std::to_string(sId) + " already exists!";
                                statusColor = sf::Color(190, 60, 60);
                            } else {
                                Section s;
                                s.sectionID = sId;
                                s.name = txtNewSectionName.getText();
                                manager.sections.add(s);
                                manager.linkData();
                                txtNewSectionId.clear();
                                txtNewSectionName.clear();
                                txtNewSectionSize.clear();
                                statusBanner = "Added Section: " + s.name + " (ID " + std::to_string(sId) + ")";
                                statusColor = sf::Color(40, 140, 90);
                            }
                        }
                    }
                }
                else if (currentScreen == Screen::TIMETABLE) {
                    int totalSections = manager.sections.size();
                    if (btnPrevSection.isClicked(mousePos) && totalSections > 0) {
                        selectedSectionIdx--;
                        if (selectedSectionIdx < -1) selectedSectionIdx = totalSections - 1;
                    }
                    else if (btnNextSection.isClicked(mousePos) && totalSections > 0) {
                        selectedSectionIdx++;
                        if (selectedSectionIdx >= totalSections) selectedSectionIdx = -1;
                    }
                    else if (btnRegenerate.isClicked(mousePos)) {
                        if (manager.getCourses().size() > 0 && manager.rooms.size() > 0) {
                            manager.resetAllAvailability();
                            timetableGenerated = solver.generateTimetable(manager);
                            statusBanner = timetableGenerated
                                ? "Timetable re-solved and sorted via MergeSort."
                                : "No valid timetable found.";
                            statusColor = timetableGenerated ? sf::Color(40, 140, 90) : sf::Color(190, 60, 60);
                        }
                    }
                }
                else if (currentScreen == Screen::SEARCH_COURSE) {
                    txtSearchId.handleClick(mousePos);
                    if (btnSearchCourse.isClicked(mousePos)) {
                        if (!txtSearchId.isEmpty()) {
                            int id = std::stoi(txtSearchId.getText());
                            searchedCourse = courseTree.search(id);
                            hasSearched = true;
                        }
                    }
                }
                else if (currentScreen == Screen::ADD_COURSE) {
                    txtCourseId.handleClick(mousePos);
                    txtCourseName.handleClick(mousePos);
                    txtCreditHours.handleClick(mousePos);
                    txtTeacherId.handleClick(mousePos);
                    txtSectionId.handleClick(mousePos);
                    txtLecturesPerWeek.handleClick(mousePos);

                    if (btnToggleLab.isClicked(mousePos)) {
                        newCourseIsLab = !newCourseIsLab;
                        btnToggleLab.setText(newCourseIsLab ? "Type: Lab Session" : "Type: Theory Lecture");
                    }
                    else if (btnSubmitCourse.isClicked(mousePos)) {
                        if (txtCourseId.isEmpty() || txtCourseName.isEmpty() ||
                            txtCreditHours.isEmpty() || txtTeacherId.isEmpty() ||
                            txtSectionId.isEmpty() || txtLecturesPerWeek.isEmpty()) {
                            statusBanner = "Please fill in all course fields before submitting.";
                            statusColor = sf::Color(190, 60, 60);
                        } else {
                            int cId = std::stoi(txtCourseId.getText());
                            if (courseTree.search(cId) != nullptr) {
                                statusBanner = "Course ID " + std::to_string(cId) + " already exists in AVL Tree!";
                                statusColor = sf::Color(190, 60, 60);
                            } else {
                                CourseOffering* newCourse = new CourseOffering();
                                newCourse->courseID = cId;
                                newCourse->courseName = txtCourseName.getText();
                                newCourse->creditHours = std::stoi(txtCreditHours.getText());
                                newCourse->teacherID = std::stoi(txtTeacherId.getText());
                                newCourse->roomID = -1;
                                newCourse->sectionID = std::stoi(txtSectionId.getText());
                                newCourse->isLab = newCourseIsLab;
                                newCourse->lecturesPerWeek = std::stoi(txtLecturesPerWeek.getText());

                                manager.courses.add(newCourse);
                                manager.courseOfferings.insertEnd(newCourse);
                                manager.linkData();
                                courseTree.insert(newCourse);

                                txtCourseId.clear();
                                txtCourseName.clear();
                                txtCreditHours.clear();
                                txtTeacherId.clear();
                                txtSectionId.clear();
                                txtLecturesPerWeek.clear();

                                if (newCourse->teacher == nullptr || newCourse->section == nullptr) {
                                    statusBanner = "Added course, but Teacher ID or Section ID was not found!";
                                    statusColor = sf::Color(190, 130, 30);
                                } else {
                                    statusBanner = "Added '" + newCourse->courseName + "' & linked pointers!";
                                    statusColor = sf::Color(40, 140, 90);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GUI::updateHoverStates(sf::Vector2f mousePos) {
    btnNavDashboard.updateHover(mousePos);
    btnNavSetup.updateHover(mousePos);
    btnNavAdd.updateHover(mousePos);
    btnNavTimetable.updateHover(mousePos);
    btnNavSearch.updateHover(mousePos);

    if (currentScreen == Screen::DASHBOARD) {
        btnGenerate.updateHover(mousePos);
        btnResetAll.updateHover(mousePos);
    }
    else if (currentScreen == Screen::SETUP_DATA) {
        txtNewTeacherId.updateHover(mousePos);
        txtNewTeacherName.updateHover(mousePos);
        btnAddTeacher.updateHover(mousePos);
        txtNewRoomId.updateHover(mousePos);
        txtNewRoomName.updateHover(mousePos);
        txtNewRoomCap.updateHover(mousePos);
        btnToggleRoomLab.updateHover(mousePos);
        btnAddRoom.updateHover(mousePos);
        txtNewSectionId.updateHover(mousePos);
        txtNewSectionName.updateHover(mousePos);
        txtNewSectionSize.updateHover(mousePos);
        btnAddSection.updateHover(mousePos);
    }
    else if (currentScreen == Screen::TIMETABLE) {
        btnPrevSection.updateHover(mousePos);
        btnNextSection.updateHover(mousePos);
        btnRegenerate.updateHover(mousePos);
    }
    else if (currentScreen == Screen::SEARCH_COURSE) {
        txtSearchId.updateHover(mousePos);
        btnSearchCourse.updateHover(mousePos);
    }
    else if (currentScreen == Screen::ADD_COURSE) {
        txtCourseId.updateHover(mousePos);
        txtCourseName.updateHover(mousePos);
        txtCreditHours.updateHover(mousePos);
        txtTeacherId.updateHover(mousePos);
        txtSectionId.updateHover(mousePos);
        txtLecturesPerWeek.updateHover(mousePos);
        btnToggleLab.updateHover(mousePos);
        btnSubmitCourse.updateHover(mousePos);
    }
}

void GUI::render() {
    window.clear(sf::Color(242, 245, 250));

    drawTopNav();

    switch (currentScreen) {
        case Screen::DASHBOARD:     drawDashboard();    
            break;
        case Screen::SETUP_DATA:    drawSetupData();    
            break;
        case Screen::TIMETABLE:     drawTimetable();    
            break;
        case Screen::SEARCH_COURSE: drawSearchCourse(); 
            break;
        case Screen::ADD_COURSE:    drawAddCourse();    
            break;
    }

    drawStatusBanner();
    window.display();
}

void GUI::drawTopNav() {
    sf::RectangleShape navBg({1280.f, 80.f});
    navBg.setFillColor(sf::Color(25, 35, 55));
    window.draw(navBg);

    btnNavDashboard.draw(window);
    btnNavSetup.draw(window);
    btnNavAdd.draw(window);
    btnNavTimetable.draw(window);
    btnNavSearch.draw(window);

    sf::Text title(font, "DS Timetable Engine", 20);
    title.setFillColor(sf::Color(200, 215, 240));
    title.setPosition({1020.f, 26.f});
    window.draw(title);
}

void GUI::drawStatusBanner() {
    sf::RectangleShape bar({1280.f, 40.f});
    bar.setPosition({0.f, 720.f});
    bar.setFillColor(statusColor);
    window.draw(bar);

    sf::Text msg(font, statusBanner, 16);
    msg.setFillColor(sf::Color::White);
    msg.setPosition({25.f, 729.f});
    window.draw(msg);
}

void GUI::drawDashboard() {
    sf::Text header(font, "System Overview & Controls", 28);
    header.setFillColor(sf::Color(30, 40, 60));
    header.setPosition({80.f, 110.f});
    window.draw(header);

    sf::Text sub(font, "Backtracking CSP Solver  |  AVL Course Index  |  MergeSort Scheduler", 17);
    sub.setFillColor(sf::Color(100, 110, 130));
    sub.setPosition({80.f, 152.f});
    window.draw(sub);

    btnGenerate.draw(window);
    btnResetAll.draw(window);

    // 4 Stat Cards showing live DataManager counts
    std::string labels[4] = {"Teachers", "Rooms & Labs", "Sections", "Course Offerings"};
    int counts[4] = {
        manager.teachers.size(),
        manager.rooms.size(),
        manager.sections.size(),
        manager.getCourses().size()
    };

    for (int i = 0; i < 4; i++) {
        float x = 80.f + i * 285.f;
        sf::RectangleShape card({260.f, 150.f});
        card.setPosition({x, 340.f});
        card.setFillColor(sf::Color::White);
        card.setOutlineThickness(1.5f);
        card.setOutlineColor(sf::Color(210, 218, 230));
        window.draw(card);

        sf::Text numTxt(font, std::to_string(counts[i]), 44);
        numTxt.setFillColor(sf::Color(45, 110, 210));
        numTxt.setPosition({x + 25.f, 360.f});
        window.draw(numTxt);

        sf::Text lblTxt(font, labels[i], 18);
        lblTxt.setFillColor(sf::Color(80, 90, 110));
        lblTxt.setPosition({x + 25.f, 435.f});
        window.draw(lblTxt);
    }
}

void GUI::drawSetupData() {
    sf::Text header(font, "Setup Teachers, Rooms & Sections", 26);
    header.setFillColor(sf::Color(30, 40, 60));
    header.setPosition({60.f, 105.f});
    window.draw(header);

    // Column Headers
    sf::Text col1(font, "Add Teacher (" + std::to_string(manager.teachers.size()) + ")", 20);
    col1.setFillColor(sf::Color(45, 110, 210));
    col1.setPosition({60.f, 160.f});
    window.draw(col1);

    sf::Text col2(font, "Add Room / Lab (" + std::to_string(manager.rooms.size()) + ")", 20);
    col2.setFillColor(sf::Color(45, 110, 210));
    col2.setPosition({440.f, 160.f});
    window.draw(col2);

    sf::Text col3(font, "Add Section (" + std::to_string(manager.sections.size()) + ")", 20);
    col3.setFillColor(sf::Color(45, 110, 210));
    col3.setPosition({820.f, 160.f});
    window.draw(col3);

    // Draw Inputs & Buttons
    txtNewTeacherId.draw(window);
    txtNewTeacherName.draw(window);
    btnAddTeacher.draw(window);

    txtNewRoomId.draw(window);
    txtNewRoomName.draw(window);
    txtNewRoomCap.draw(window);
    btnToggleRoomLab.draw(window);
    btnAddRoom.draw(window);

    txtNewSectionId.draw(window);
    txtNewSectionName.draw(window);
    txtNewSectionSize.draw(window);
    btnAddSection.draw(window);

    // Live Summary Box at Bottom
    sf::RectangleShape summaryBox({1080.f, 200.f});
    summaryBox.setPosition({60.f, 490.f});
    summaryBox.setFillColor(sf::Color::White);
    summaryBox.setOutlineThickness(1.5f);
    summaryBox.setOutlineColor(sf::Color(210, 218, 230));
    window.draw(summaryBox);

    std::string tList = "Teachers: ";
    for (int i = 0; i < manager.teachers.size(); i++) {
        tList += "[" + std::to_string(manager.teachers.get(i).teacherID) + ": " + manager.teachers.get(i).name + "] ";
    }
    std::string rList = "Rooms: ";
    for (int i = 0; i < manager.rooms.size(); i++) {
        rList += "[" + std::to_string(manager.rooms.get(i).roomID) + ": " + manager.rooms.get(i).roomName +
                 (manager.rooms.get(i).isLab ? " (Lab)" : "") + "] ";
    }
    std::string sList = "Sections: ";
    for (int i = 0; i < manager.sections.size(); i++) {
        sList += "[" + std::to_string(manager.sections.get(i).sectionID) + ": " + manager.sections.get(i).name + "] ";
    }

    sf::Text listTxt(font, tList + "\n\n" + rList + "\n\n" + sList, 15);
    listTxt.setFillColor(sf::Color(50, 60, 80));
    listTxt.setPosition({80.f, 510.f});
    window.draw(listTxt);
}

void GUI::drawTimetable() {
    std::string filterTitle = "Weekly Timetable (All Sections)";
    if (selectedSectionIdx >= 0 && selectedSectionIdx < manager.sections.size()) {
        filterTitle = "Weekly Timetable - Section: " + manager.sections.get(selectedSectionIdx).name;
    }

    sf::Text header(font, filterTitle, 24);
    header.setFillColor(sf::Color(30, 40, 60));
    header.setPosition({40.f, 102.f});
    window.draw(header);

    btnPrevSection.draw(window);
    btnNextSection.draw(window);
    btnRegenerate.draw(window);

    const char* days[5] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    const char* slots[4] = {
        "Slot 1\n08:30 - 10:00",
        "Slot 2\n10:15 - 11:45",
        "Slot 3\n12:00 - 01:30",
        "Slot 4\n02:00 - 03:30"
    };

    float startX = 175.f;
    float startY = 160.f;
    float colW = 212.f;
    float rowH = 128.f;

    // Column Headers (Days)
    for (int d = 0; d < 5; d++) {
        sf::RectangleShape cell({colW - 4.f, 40.f});
        cell.setPosition({startX + d * colW, startY});
        cell.setFillColor(sf::Color(45, 110, 210));
        window.draw(cell);

        sf::Text dayTxt(font, days[d], 17);
        dayTxt.setFillColor(sf::Color::White);
        dayTxt.setPosition({startX + d * colW + 15.f, startY + 9.f});
        window.draw(dayTxt);
    }

    // Row Headers (Time Slots) & Background Grid
    for (int s = 0; s < 4; s++) {
        float y = startY + 46.f + s * rowH;

        sf::RectangleShape slotBox({130.f, rowH - 6.f});
        slotBox.setPosition({35.f, y});
        slotBox.setFillColor(sf::Color(225, 232, 245));
        window.draw(slotBox);

        sf::Text slotTxt(font, slots[s], 14);
        slotTxt.setFillColor(sf::Color(40, 50, 70));
        slotTxt.setPosition({45.f, y + 35.f});
        window.draw(slotTxt);

        for (int d = 0; d < 5; d++) {
            sf::RectangleShape emptyCell({colW - 4.f, rowH - 6.f});
            emptyCell.setPosition({startX + d * colW, y});
            emptyCell.setFillColor(sf::Color::White);
            emptyCell.setOutlineThickness(1.f);
            emptyCell.setOutlineColor(sf::Color(215, 222, 235));
            window.draw(emptyCell);
        }
    }

    // Populate Scheduled Entries
    ScheduleArray& sched = manager.getFinalSchedule();
    for (int i = 0; i < sched.size(); i++) {
        ScheduleEntry& e = sched.get(i);
        int d = e.getDay();
        int s = e.getSlot();
        if (d < 0 || d >= 5 || s < 0 || s >= 4) continue;

        CourseOffering* c = courseTree.search(e.courseID);

        // Apply section filter if active
        if (selectedSectionIdx >= 0 && selectedSectionIdx < manager.sections.size()) {
            int targetSecId = manager.sections.get(selectedSectionIdx).sectionID;
            if (c == nullptr || c->section == nullptr || c->section->sectionID != targetSecId) {
                continue;
            }
        }

        float cellX = startX + d * colW + 4.f;
        float cellY = startY + 46.f + s * rowH + 4.f;

        sf::RectangleShape card({colW - 12.f, rowH - 14.f});
        card.setPosition({cellX, cellY});
        card.setFillColor((c != nullptr && c->isLab) ? sf::Color(235, 245, 255) : sf::Color(235, 250, 240));
        card.setOutlineThickness(1.5f);
        card.setOutlineColor((c != nullptr && c->isLab) ? sf::Color(60, 130, 220) : sf::Color(50, 160, 100));
        window.draw(card);

        std::string cName = (c != nullptr) ? c->courseName : ("Course #" + std::to_string(e.courseID));
        std::string info = cName + "\n"
                         + "Room: " + getRoomName(e.roomID) + "\n"
                         + "Prof: " + getTeacherName(e.teacherID) + "\n"
                         + "Sec: " + getSectionName(c);

        sf::Text entryTxt(font, info, 13);
        entryTxt.setFillColor(sf::Color(25, 35, 50));
        entryTxt.setPosition({cellX + 8.f, cellY + 6.f});
        window.draw(entryTxt);
    }
}

void GUI::drawSearchCourse() {
    sf::Text header(font, "AVL Tree Course Lookup (O(log n))", 26);
    header.setFillColor(sf::Color(30, 40, 60));
    header.setPosition({80.f, 115.f});
    window.draw(header);

    txtSearchId.draw(window);
    btnSearchCourse.draw(window);

    // Result Card
    sf::RectangleShape card({650.f, 260.f});
    card.setPosition({80.f, 260.f});
    card.setFillColor(sf::Color::White);
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(sf::Color(210, 218, 230));
    window.draw(card);

    if (!hasSearched) {
        sf::Text hint(font, "Enter a Course ID above and click Search AVL Tree.", 18);
        hint.setFillColor(sf::Color(120, 130, 150));
        hint.setPosition({110.f, 370.f});
        window.draw(hint);
    }
    else if (searchedCourse == nullptr) {
        sf::Text notFound(font, "Course ID not found in AVL Tree.", 20);
        notFound.setFillColor(sf::Color(200, 60, 60));
        notFound.setPosition({110.f, 370.f});
        window.draw(notFound);
    }
    else {
        std::string details =
            "Course ID:        " + std::to_string(searchedCourse->courseID) + "\n\n" +
            "Course Name:      " + searchedCourse->courseName + "\n" +
            "Credit Hours:     " + std::to_string(searchedCourse->creditHours) + "\n" +
            "Lectures / Week:  " + std::to_string(searchedCourse->lecturesPerWeek) + "\n" +
            "Session Type:     " + std::string(searchedCourse->isLab ? "Lab" : "Theory") + "\n" +
            "Assigned Teacher: " + getTeacherName(searchedCourse->teacherID) + "\n" +
            "Section:          " + getSectionName(searchedCourse);

        sf::Text resTxt(font, details, 18);
        resTxt.setFillColor(sf::Color(30, 40, 60));
        resTxt.setPosition({110.f, 285.f});
        window.draw(resTxt);
    }
}

void GUI::drawAddCourse() {
    sf::Text header(font, "Add New Course Offering", 26);
    header.setFillColor(sf::Color(30, 40, 60));
    header.setPosition({80.f, 110.f});
    window.draw(header);

    txtCourseId.draw(window);
    txtCourseName.draw(window);
    txtCreditHours.draw(window);
    txtTeacherId.draw(window);
    txtSectionId.draw(window);
    txtLecturesPerWeek.draw(window);
    btnToggleLab.draw(window);
    btnSubmitCourse.draw(window);

    // Reference Sidebar showing valid Teacher IDs and Section IDs from DataManager
    sf::RectangleShape refBox({340.f, 340.f});
    refBox.setPosition({870.f, 170.f});
    refBox.setFillColor(sf::Color::White);
    refBox.setOutlineThickness(1.5f);
    refBox.setOutlineColor(sf::Color(210, 218, 230));
    window.draw(refBox);

    std::string refStr = "Available IDs in DataManager:\n\nTeachers:\n";
    for (int i = 0; i < manager.teachers.size(); i++) {
        refStr += "  ID " + std::to_string(manager.teachers.get(i).teacherID) + ": " + manager.teachers.get(i).name + "\n";
    }
    refStr += "\nSections:\n";
    for (int i = 0; i < manager.sections.size(); i++) {
        refStr += "  ID " + std::to_string(manager.sections.get(i).sectionID) + ": " + manager.sections.get(i).name + "\n";
    }

    sf::Text refTxt(font, refStr, 15);
    refTxt.setFillColor(sf::Color(50, 60, 80));
    refTxt.setPosition({890.f, 185.f});
    window.draw(refTxt);
}