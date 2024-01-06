object Form3: TForm3
  Left = 189
  Top = 162
  Caption = 'Aitotal'
  ClientHeight = 375
  ClientWidth = 969
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  Visible = True
  OnActivate = FormActivate
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 969
    Height = 77
    Align = alTop
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 0
    DesignSize = (
      969
      77)
    object Label1: TLabel
      Left = 4
      Top = 15
      Width = 77
      Height = 16
      Caption = #1042#1074#1077#1076#1080#1090#1077' MD5'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object ChReSca: TCheckBox
      Left = 881
      Top = 41
      Width = 61
      Height = 17
      Anchors = [akTop, akRight]
      Caption = 'ReScan'
      Checked = True
      State = cbChecked
      TabOrder = 4
    end
    object Button1: TButton
      Left = 881
      Top = 10
      Width = 84
      Height = 25
      Anchors = [akTop, akRight]
      Caption = #1057#1090#1086#1087
      TabOrder = 2
      OnClick = Button1Click
    end
    object ProgressBar1: TProgressBar
      Left = 4
      Top = 50
      Width = 871
      Height = 15
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 5
    end
    object Edit1: TEdit
      Left = 87
      Top = 12
      Width = 621
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnKeyPress = Edit1KeyPress
    end
    object Button3: TButton
      Left = 792
      Top = 10
      Width = 83
      Height = 25
      Hint = #1042#1074#1077#1076#1080#1090#1077' '#1093#1077#1096#1089#1091#1084#1084#1091' MD5'
      Anchors = [akTop, akRight]
      Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100
      TabOrder = 1
      OnClick = Button3Click
    end
    object Button2: TButton
      Left = 617
      Top = 19
      Width = 75
      Height = 25
      Caption = 'Button2'
      TabOrder = 0
      Visible = False
      OnClick = Button2Click
    end
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 77
    Width = 969
    Height = 246
    ActivePage = TabSheet1
    Align = alClient
    DoubleBuffered = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Reference Sans Serif'
    Font.Style = []
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = #1060#1072#1081#1083' '#1087#1088#1086#1074#1077#1088#1082#1080
      object ListView1: TListView
        Left = 0
        Top = 0
        Width = 961
        Height = 216
        ParentCustomHint = False
        Align = alClient
        Checkboxes = True
        Columns = <
          item
            Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
            Width = 145
          end
          item
            Caption = #1055#1086#1083#1085#1086#1077' '#1080#1084#1103' '#1092#1072#1081#1083#1072
            Tag = 1
            Width = 232
          end
          item
            Caption = #1056#1072#1079#1084#1077#1088
            Tag = 2
            Width = 85
          end
          item
            Caption = 'MD5'
            Tag = 3
            Width = 188
          end
          item
            AutoSize = True
            Caption = 'SHA256'
            MinWidth = 295
            Tag = 4
          end>
        DoubleBuffered = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Reference Sans Serif'
        Font.Style = []
        GridLines = True
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        ParentDoubleBuffered = False
        ParentFont = False
        PopupMenu = PopupMenu1
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = ListView1ColumnClick
        OnCustomDrawItem = ListView1CustomDrawItem
        OnCustomDrawSubItem = ListView1CustomDrawSubItem
        OnDblClick = LV1Diblklic
        OnDeletion = LW1OnDeletion
        OnInsert = LW1OnInsert
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1057#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1077
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ListView2: TListView
        Left = 0
        Top = 0
        Width = 961
        Height = 216
        Align = alClient
        Checkboxes = True
        Columns = <
          item
            Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
            Width = 80
          end
          item
            Caption = #1055#1086#1083#1085#1086#1077' '#1080#1084#1103' '#1092#1072#1081#1083#1072
            Tag = 1
            Width = 150
          end
          item
            Caption = #1056#1072#1079#1084#1077#1088
            Tag = 2
            Width = 85
          end
          item
            Caption = 'MD5 '#1092#1072#1081#1083#1072
            Tag = 3
            Width = 80
          end
          item
            Caption = 'SHA256'
            Tag = 4
          end
          item
            Caption = '% '#1047#1072#1075#1088#1091#1079#1082#1080'\'#1087#1088#1086#1074#1077#1088#1082#1080
            MinWidth = 110
            Tag = 5
            Width = 219
          end
          item
            AutoSize = True
            Caption = #1057#1089#1099#1083#1082#1072' '#1085#1072' '#1086#1090#1095#1105#1090
            Tag = 6
          end>
        DoubleBuffered = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Reference Sans Serif'
        Font.Style = []
        GridLines = True
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        ParentDoubleBuffered = False
        ParentFont = False
        PopupMenu = PopupMenu2
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = ListView2ColumnClick
        OnCustomDrawItem = ListView2CustomDrawItem
        OnCustomDrawSubItem = ListView2CustomDrawSubItem
        OnDblClick = LV2Dblklic
        OnDeletion = LW2OnDeletion
        OnInsert = LE2OnInsert
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ListView3: TListView
        Left = 0
        Top = 0
        Width = 961
        Height = 216
        Align = alClient
        Checkboxes = True
        Columns = <
          item
            Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
          end
          item
            Caption = #1055#1086#1083#1085#1086#1077' '#1080#1084#1103' '#1092#1072#1081#1083#1072
            Tag = 1
          end
          item
            Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
            Tag = 2
            Width = 70
          end
          item
            Caption = #1056#1072#1079#1084#1077#1088
            Tag = 3
          end
          item
            Caption = 'MD5'
            Tag = 4
          end
          item
            Caption = 'SHA256'
            Tag = 5
          end
          item
            Caption = #1044#1072#1090#1072' '#1087#1088#1086#1074#1077#1088#1082#1080
            Tag = 6
          end
          item
            AutoSize = True
            Caption = #1057#1089#1099#1083#1082#1072' '#1085#1072' '#1086#1090#1095#1105#1090
            MinWidth = 150
            Tag = 7
          end>
        DoubleBuffered = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Reference Sans Serif'
        Font.Style = []
        GridLines = True
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        ParentDoubleBuffered = False
        ParentFont = False
        PopupMenu = PopupMenu3
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = ListView3ColumnClick
        OnCustomDrawItem = ListView3CustomDrawItem
        OnCustomDrawSubItem = ListView3CustomDrawSubItem
        OnDblClick = LV3PokaztRelt
        OnDeletion = LW3OnDeletions
        OnInsert = LW3OnInsert
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 323
    Width = 969
    Height = 52
    Align = alBottom
    TabOrder = 2
    object TLabelItogo: TLabel
      Left = 1
      Top = 1
      Width = 280
      Height = 26
      Align = alLeft
      AutoSize = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Reference Sans Serif'
      Font.Style = []
      ParentFont = False
      ExplicitHeight = 32
    end
    object TLabelCountProwerki: TLabel
      Left = 281
      Top = 1
      Width = 429
      Height = 26
      Align = alClient
      AutoSize = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
      ExplicitLeft = 287
      ExplicitTop = 2
    end
    object TLabelCountProwereno: TLabel
      Left = 710
      Top = 1
      Width = 258
      Height = 26
      Align = alRight
      AutoSize = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
      ExplicitLeft = 618
      ExplicitHeight = 32
    end
    object stat1: TStatusBar
      Left = 1
      Top = 27
      Width = 967
      Height = 24
      Panels = <
        item
          Width = 500
        end>
    end
  end
  object MainMenu1: TMainMenu
    Left = 29
    Top = 157
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N3: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1060#1072#1081#1083
        OnClick = MmOpenFile
      end
      object N4: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1055#1072#1087#1082#1091
        OnClick = MmOpenDir
      end
      object N55: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1072#1087#1082#1091' '#1089' '#1082#1072#1088#1072#1085#1090#1080#1085#1086#1084
        OnClick = MM1ScanFolderArchiv
      end
      object N59: TMenuItem
        Caption = #1042#1086#1089#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1089#1077#1089#1089#1080#1102
        OnClick = ScanRevers
      end
      object N61: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1089#1077#1089#1089#1080#1102'.'
        OnClick = SaveSession
      end
      object N60: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1089#1077#1089#1089#1080#1102' '#1082#1072#1082'...'
        OnClick = SaveSessionKak
      end
      object N2: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = MmExit
      end
    end
    object N12: TMenuItem
      Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086
      object N13: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
        OnClick = MyTools
      end
      object N47: TMenuItem
        Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
        OnClick = About
      end
      object N56: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1074' '#1082#1086#1085#1090#1077#1082#1089#1090#1085#1086#1077' '#1084#1077#1085#1102' '#1087#1088#1086#1074#1086#1076#1085#1080#1082#1072
        Visible = False
        OnClick = InsertToReestrMenu
      end
      object N57: TMenuItem
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1080#1079' '#1082#1086#1085#1090#1077#1082#1089#1090#1085#1086#1075#1086' '#1084#1077#1085#1102' '#1087#1088#1086#1074#1086#1076#1085#1080#1082#1072
        Visible = False
        OnClick = DeleteReestr
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 21
    Top = 231
    object N6: TMenuItem
      Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100
      OnClick = Pm1Scan
    end
    object N42: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      object N43: TMenuItem
        Caption = #1048#1084#1103' '#1060#1072#1081#1083#1072
        OnClick = LW1CopiNameFile
      end
      object N44: TMenuItem
        Caption = #1055#1091#1090#1100' '#1082' '#1092#1072#1081#1083#1091
        OnClick = LW1CopiPath
      end
      object N45: TMenuItem
        Caption = #1055#1086#1083#1085#1099#1081' '#1087#1091#1090#1100' '#1082' '#1092#1072#1081#1083#1091
        OnClick = LW1CopiFileNamePatch
      end
      object N46: TMenuItem
        Caption = #1056#1072#1079#1084#1077#1088' '#1092#1072#1081#1083#1072
        OnClick = LW1CopiFileSize
      end
      object MD52: TMenuItem
        Caption = 'MD5'
        OnClick = LW1CopiMD5
      end
      object SHA2562: TMenuItem
        Caption = 'SHA256'
        OnClick = LV1CopySHA256
      end
    end
    object N31: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1072#1087#1082#1091' '#1089' '#1092#1072#1081#1083#1086#1084
      OnClick = Pm1OpenDirFile
    end
    object N7: TMenuItem
      Caption = #1057#1085#1103#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1080#1103
      OnClick = Pm1Reset
    end
    object N5: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1105
      OnClick = Pm1Select
    end
    object N15: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = Pm1Delete
    end
    object N8: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077' '#1092#1072#1081#1083#1099
      OnClick = Pm1DeleteChecked
    end
    object N24: TMenuItem
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1080#1103
      OnClick = Pm1OtmenaVideleniya
    end
    object N51: TMenuItem
      Caption = #1057#1074#1086#1081#1089#1090#1074#1072' '#1092#1072#1081#1083#1072
      OnClick = Pm1FileInfo
    end
    object N52: TMenuItem
      Caption = #1054#1090#1084#1077#1090#1080#1090#1100' '#1075#1072#1083#1086#1095#1082#1086#1081' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077
      OnClick = Pm1SelectedToCheked
    end
  end
  object PopupMenu2: TPopupMenu
    Left = 96
    Top = 236
    object N38: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      object N39: TMenuItem
        Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
        OnClick = LW2CopiNameFile
      end
      object N40: TMenuItem
        Caption = #1055#1091#1090#1100' '#1082' '#1092#1072#1081#1083#1091
        OnClick = LW2CopiPath
      end
      object N28: TMenuItem
        Caption = #1055#1086#1083#1085#1086#1077' '#1080#1084#1103' '#1092#1072#1081#1083#1072
        OnClick = LW2CopiNameFilePath
      end
      object N41: TMenuItem
        Caption = #1056#1072#1079#1084#1077#1088
        OnClick = LW2CopiSizeFile
      end
      object MD51: TMenuItem
        Caption = 'MD5'
        OnClick = LW2CopiMD5
      end
      object SHA2563: TMenuItem
        Caption = 'SHA256'
        OnClick = LV2CopySHA256
      end
      object N48: TMenuItem
        Caption = #1057#1089#1099#1083#1082#1091' '#1085#1072' '#1086#1090#1095#1105#1090
        OnClick = LV2CopySsilka
      end
    end
    object N11: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1077
      OnClick = Pm2VidelitVse
    end
    object N26: TMenuItem
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1085#1086#1077
      OnClick = Pm2OtmenaVideleviya
    end
    object N9: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = Pm2Delete
    end
    object N10: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077' '#1092#1072#1081#1083#1099
      OnClick = Pm2DeleteChecedListView
    end
    object N30: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1072#1087#1082#1091' '#1089' '#1092#1072#1081#1083#1086#1084
      OnClick = Pm2OpenDirFile
    end
    object N35: TMenuItem
      Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100
      OnClick = LWProverit
    end
    object N36: TMenuItem
      Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077
      OnClick = Pm2ScanCheked
    end
    object N49: TMenuItem
      Caption = #1057#1090#1086#1087
      OnClick = Pm2StopSelected
    end
    object N50: TMenuItem
      Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1086#1090#1084#1077#1095#1077#1085#1085#1099#1077
      OnClick = Pm2StopCHeked
    end
    object N29: TMenuItem
      Caption = #1057#1074#1086#1081#1089#1090#1074#1072' '#1092#1072#1081#1083#1072
      OnClick = Pm2FileInfo
    end
    object N53: TMenuItem
      Caption = #1054#1090#1084#1077#1090#1080#1090#1100' '#1075#1072#1083#1086#1095#1082#1086#1081' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077
      OnClick = Pm2SelectedToCheked
    end
    object N58: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1054#1090#1095#1105#1090'.'
      OnClick = Pm2SaveLog
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 828
    Top = 114
  end
  object PopupMenu3: TPopupMenu
    Left = 171
    Top = 232
    object N23: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1086#1090#1095#1105#1090' '#1074' '#1073#1088#1072#1091#1079#1077#1088#1077
      OnClick = Pm3OpenReZultat
    end
    object N37: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1086#1090#1095#1105#1090' '#1074' '#1073#1088#1072#1091#1079#1077#1088#1077' '#1087#1086' '#1074#1099#1073#1088#1072#1085#1085#1099#1084' '#1092#1072#1081#1083#1072#1084
      OnClick = Pm3OpenReZultCheked
    end
    object N16: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      object N17: TMenuItem
        Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
        OnClick = Pm3CopyNameFile
      end
      object N18: TMenuItem
        Caption = #1055#1091#1090#1100' '#1082' '#1092#1072#1081#1083#1091
        OnClick = Pm3CopyPatchFiele
      end
      object N19: TMenuItem
        Caption = #1055#1086#1083#1085#1099#1081' '#1087#1091#1090#1100' '#1082' '#1092#1072#1081#1083#1091
        OnClick = Pm3CopyPatchNameFile
      end
      object Hesh1: TMenuItem
        Caption = 'MD5 '#1092#1072#1081#1083#1072
        OnClick = Pm3CopyHeshMd5
      end
      object SHA2561: TMenuItem
        Caption = 'SHA256 '#1092#1072#1081#1083#1072
        OnClick = SHA2561Click
      end
      object N20: TMenuItem
        Caption = #1057#1089#1099#1083#1082#1091' '#1085#1072' '#1086#1090#1095#1105#1090
        OnClick = Pm3CopyAdres
      end
    end
    object N32: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1042#1089#1077
      OnClick = Pm3VidelitVse
    end
    object Pm3OtmeVidelit1: TMenuItem
      Caption = #1057#1085#1103#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1080#1077
      OnClick = Pm3OtmenaVidelit
    end
    object N14: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = Pm3Delete
    end
    object N25: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077' '#1092#1072#1081#1083#1099
      OnClick = Pm3DeleteCheked
    end
    object N21: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1072#1087#1082#1091' '#1089' '#1092#1072#1081#1083#1086#1084
      OnClick = Pm3OpenDirFile
    end
    object N27: TMenuItem
      Caption = #1057#1074#1086#1081#1089#1090#1074#1072' '#1092#1072#1081#1083#1072
      OnClick = Pm3FileInfo
    end
    object N33: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1054#1090#1095#1105#1090
      OnClick = LV3SaveRezult
    end
    object N34: TMenuItem
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      OnClick = LV3Clear
    end
    object N54: TMenuItem
      Caption = #1054#1090#1084#1077#1090#1080#1090#1100' '#1075#1072#1083#1086#1095#1082#1086#1081' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1077
      OnClick = Pm3SelectedToCheked
    end
  end
  object TrayIcon1: TTrayIcon
    BalloonHint = 'Asdf'
    BalloonTitle = 'Derfah'
    BalloonTimeout = 3
    PopupMenu = PopupTrayIcon
    Visible = True
    OnBalloonClick = TrayIcon1BalloonClick
    OnClick = TreyIc
    Left = 612
    Top = 158
  end
  object PopupTrayIcon: TPopupMenu
    Left = 245
    Top = 231
    object N22: TMenuItem
      Caption = #1042#1099#1093#1086#1076
      OnClick = MmExit
    end
  end
  object SaveDialog1: TSaveDialog
    FileName = 'log.csv'
    Filter = '|*.csv|'
    Left = 823
    Top = 178
  end
  object IdSSLIOHandlerSocketOpenSSL1: TIdSSLIOHandlerSocketOpenSSL
    MaxLineAction = maException
    Port = 0
    DefaultPort = 0
    SSLOptions.Mode = sslmUnassigned
    SSLOptions.VerifyMode = []
    SSLOptions.VerifyDepth = 0
    Left = 752
    Top = 226
  end
  object SaveJeson: TSaveDialog
    Filter = '|*.json'
    Left = 912
    Top = 106
  end
  object OpenJeson: TOpenDialog
    Filter = 'jesson|*.json'
    Left = 900
    Top = 170
  end
end
