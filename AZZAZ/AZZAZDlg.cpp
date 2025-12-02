#include "pch.h"
#include "framework.h"
#include "AZZAZ.h"
#include "AZZAZDlg.h"
#include "afxdialogex.h"
#include "Node.h"
#include "ExpressionTokenizer.h"
#include "ExpressionParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------- Boîte de dialogue "À propos" ----------------
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx) END_MESSAGE_MAP()

// ---------------- Boîte de dialogue principale ----------------
CAZZAZDlg::CAZZAZDlg(CWnd* pParent) : CDialogEx(IDD_AZZAZ_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // Initialisation de GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

CAZZAZDlg::~CAZZAZDlg()
{
    if (arbreGlobal) { delete arbreGlobal; arbreGlobal = nullptr; }

    // Arrêt de GDI+
    GdiplusShutdown(gdiplusToken);
}

void CAZZAZDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAZZAZDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_SIMULER, &CAZZAZDlg::OnBnClickedButtonSimuler)
END_MESSAGE_MAP()

BOOL CAZZAZDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;
}

void CAZZAZDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

HCURSOR CAZZAZDlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }

// ---------------- Dessiner un PNG ----------------
void DrawGatePNG(CDC* pDC, int x, int y, LPCWSTR filePath, int width, int height)
{
    Graphics graphics(pDC->GetSafeHdc());
    Image img(filePath);
    graphics.DrawImage(&img, x, y, width, height);
}

// ---------------- Dessin de l'arbre logique avec PNG ----------------
void CAZZAZDlg::DrawLogicTree(CDC* pDC, Node* node, int x, int y, int& nextX)
{
    if (!node) return;

    int width = 50;       // largeur des portes légèrement réduite
    int height = 50;      // hauteur uniforme pour portes et inputs
    int levelGap = 120;   // espace vertical entre niveaux
    int siblingGap = 100; // espace horizontal entre portes ou inputs frères


    GateNode* gate = dynamic_cast<GateNode*>(node);
    InputNode* input = dynamic_cast<InputNode*>(node);

    int childY = y + levelGap;
    int leftX = nextX;

    if (gate)
    {
        DrawLogicTree(pDC, gate->left, nextX, childY, nextX);
        int rightX = nextX;
        if (gate->right) DrawLogicTree(pDC, gate->right, nextX, childY, nextX);

        int centerX = gate->right ? (leftX + nextX - 2*siblingGap)/2 : leftX;

        // Construire le chemin complet de l'image
        WCHAR exePath[MAX_PATH];
        GetModuleFileName(nullptr, exePath, MAX_PATH);
        PathRemoveFileSpec(exePath);
        CString imgPath;
        if (gate->type == L"AND") imgPath.Format(L"%s\\Resources\\Gates\\AND.png", exePath);
        else if (gate->type == L"OR") imgPath.Format(L"%s\\Resources\\Gates\\OR.png", exePath);
        else if (gate->type == L"XOR") imgPath.Format(L"%s\\Resources\\Gates\\XOR.png", exePath);
        else if (gate->type == L"NOT") imgPath.Format(L"%s\\Resources\\Gates\\NOT.png", exePath);

        if (!imgPath.IsEmpty())
            DrawGatePNG(pDC, centerX - width/2, y, imgPath, width, height);

        // Lignes vers enfants
        if (gate->left)
            pDC->MoveTo(centerX, y + height), pDC->LineTo(leftX, childY);
        if (gate->right)
            pDC->MoveTo(centerX, y + height), pDC->LineTo(nextX - siblingGap, childY);
    }
    else if (input)
    {
        pDC->Rectangle(nextX - width/2, y, nextX + width/2, y + height);
        pDC->TextOutW(nextX - 10, y + 5, input->name);
        nextX += siblingGap;
    }
    OutputNode* output = dynamic_cast<OutputNode*>(node);
if (output)
{
    int childY = y + levelGap;
    int childLeft = nextX;  // mémorise la position de départ pour l'enfant

    // Dessiner récursivement le fils et mettre à jour nextX
    if (output->input)
        DrawLogicTree(pDC, output->input, nextX, childY, nextX);

    // Calculer le centre réel de S : milieu entre début et fin de l'enfant
    int centerX = childLeft + (nextX - childLeft - 2*siblingGap) / 2;

    // Dessiner le carré S
    pDC->Rectangle(centerX - width / 2, y, centerX + width / 2, y + height);
    pDC->TextOutW(centerX - 10, y + 5, output->name);

    // Tracer la ligne vers l’enfant
    if (output->input)
        pDC->MoveTo(centerX, y + height),
        pDC->LineTo(centerX, childY);

    return;
}


}

// ---------------- Dessin chronogramme ----------------
void CAZZAZDlg::DrawChronogram(CDC* pDC, int leftMargin, int y,
    const std::vector<bool>& values,
    const std::vector<int>& durations,
    CString name)
{
    if (values.empty() || durations.empty()) return;

    int scale = 2;
    int amplitude = 20;
    int baseY = y + amplitude;
    int topY = y;

    // Texte centré à gauche du graphe
    CRect textRect(leftMargin - 60, y - 5, leftMargin - 10, y + amplitude + 5);
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(name, &textRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

    int currentX = leftMargin;

    for (size_t i = 0; i < values.size(); ++i)
    {
        int levelY = values[i] ? topY : baseY;
        int width = durations[i] * scale;

        pDC->MoveTo(currentX, levelY);
        pDC->LineTo(currentX + width, levelY);

        if (i < values.size() - 1 && values[i] != values[i + 1])
        {
            pDC->MoveTo(currentX + width, levelY);
            pDC->LineTo(currentX + width, values[i + 1] ? topY : baseY);
        }

        currentX += width;
    }
}



// ---------------- Lecture des signaux ----------------
void CAZZAZDlg::LoadSignalFile(const CString& filePath)
{
    timeline.clear();

    CStdioFile file;
    if (!file.Open(filePath, CFile::modeRead | CFile::typeText))
    {
        AfxMessageBox(L"Impossible d’ouvrir le fichier des signaux.");
        return;
    }

    CString line;
    TimeStep step = {0,0,0,0};

    while (file.ReadString(line))
    {
        line.Trim();
        if (line.IsEmpty()) continue;

        if (line.Left(1) == L"X" || line.Left(1) == L"Y" || line.Left(1) == L"Z")
        {
            wchar_t var;
            int val;
            if (_stscanf_s(line, L"%c = %d", &var, 1, &val) == 2)
            {
                if (var == L'X') step.X = (val != 0);
                if (var == L'Y') step.Y = (val != 0);
                if (var == L'Z') step.Z = (val != 0);
            }
        }
        else if (line.Find(L"ns") != -1)
        {
            int duration = 0;
            if (_stscanf_s(line, L"%dns", &duration) == 1)
            {
                step.duration = duration;
                timeline.push_back(step);
            }
        }
    }

    file.Close();
}

// ---------------- Bouton simuler ----------------
void CAZZAZDlg::OnBnClickedButtonSimuler()
{
    CString eqStr;
    GetDlgItemText(IDC_EDIT_EQUATION, eqStr);
    if (eqStr.IsEmpty()) {
        AfxMessageBox(L"Veuillez entrer une expression logique (ex: (X AND Y) OR (NOT Z)).");
        return;
    }

    CFileDialog fileDlg(TRUE, L"txt", nullptr, OFN_FILEMUSTEXIST, L"Fichiers texte (*.txt)|*.txt||");
    if (fileDlg.DoModal() != IDOK) return;

    LoadSignalFile(fileDlg.GetPathName());

    if (timeline.empty()) {
        AfxMessageBox(L"Fichier vide ou invalide.");
        return;
    }

    CT2A asciiEq(eqStr);
    std::string eqStd(asciiEq);

    ExpressionTokenizer tokenizer;
    auto tokens = tokenizer.Tokenize(eqStd);

    Xbits.clear();
    Ybits.clear();
    Zbits.clear();
    Sbits.clear();

    if (arbreGlobal) { delete arbreGlobal; arbreGlobal = nullptr; }

    std::map<CString,bool> vars = { {_T("X"),false},{_T("Y"),false},{_T("Z"),false} };
    ExpressionParser parser(tokens, vars);
    arbreGlobal = parser.parse();
    arbreGlobal = new OutputNode(L"S", arbreGlobal);

    for (const auto& step : timeline)
    {
        std::map<CString,bool> varsNow = { {_T("X"),step.X},{_T("Y"),step.Y},{_T("Z"),step.Z} };
        ExpressionParser parserEval(tokens, varsNow);
        Node* treeEval = parserEval.parse();
        bool sVal = treeEval->evaluate();
        delete treeEval;

        Xbits.push_back(step.X);
        Ybits.push_back(step.Y);
        Zbits.push_back(step.Z);
        Sbits.push_back(sVal);
    }

    Invalidate();
}

// ---------------- Paint principal ----------------
void CAZZAZDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
        return;
    }

    CPaintDC dc(this);

    if (!timeline.empty())
    {
        std::vector<int> durations;
        for (auto& step : timeline) durations.push_back(step.duration);

        int y = 60;                // position verticale initiale
        int leftMargin = 80;        // <-- déclaration obligatoire pour l'alignement

        // Chronogrammes
        DrawChronogram(&dc, leftMargin, y, Xbits, durations, L"X");
        DrawChronogram(&dc, leftMargin, y + 50, Ybits, durations, L"Y");
        DrawChronogram(&dc, leftMargin, y + 100, Zbits, durations, L"Z");
        DrawChronogram(&dc, leftMargin, y + 150, Sbits, durations, L"S");

        // Arbre logique
        if (arbreGlobal)
        {
            int nextX = leftMargin + 50;   // démarre légèrement décalé pour centrer l'arbre
            int logicTreeY = y + 260;
            DrawLogicTree(&dc, arbreGlobal, leftMargin, logicTreeY, nextX);
        }
    }
    else
    {
        dc.TextOutW(50, 50, L"Aucun chronogramme a afficher, Entrez ton expression");
    }
}
