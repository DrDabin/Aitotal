object FormResultScan: TFormResultScan
  Left = 262
  Top = 129
  BorderWidth = 3
  Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090' '#1087#1088#1086#1074#1077#1088#1082#1080
  ClientHeight = 413
  ClientWidth = 723
  Color = clWhite
  Constraints.MinHeight = 451
  Constraints.MinWidth = 590
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Microsoft Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 723
    Height = 81
    Align = alTop
    BorderWidth = 2
    Color = clInactiveBorder
    ParentBackground = False
    TabOrder = 0
    DesignSize = (
      723
      81)
    object LbNameFile: TLabel
      Left = 76
      Top = 9
      Width = 75
      Height = 16
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072':'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LbPathFile: TLabel
      Left = 76
      Top = 29
      Width = 81
      Height = 16
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = #1055#1091#1090#1100' '#1092#1072#1081#1083#1072':'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LbScanDate: TLabel
      Left = 76
      Top = 49
      Width = 131
      Height = 16
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = #1044#1072#1090#1072' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103':'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LBVTFileName: TLabel
      Left = 163
      Top = 9
      Width = 3
      Height = 15
      Cursor = crHandPoint
      Anchors = [akLeft, akTop, akRight, akBottom]
      Color = clBtnHighlight
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = LBSvoistvoFila
      OnMouseMove = LBVTFileNameMouseMove
      OnMouseLeave = LBVTFileNameMouseLeave
    end
    object LBVtPathFile: TLabel
      Left = 168
      Top = 29
      Width = 3
      Height = 15
      Color = clBtnHighlight
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object LBVtDataScan: TLabel
      Left = 218
      Top = 49
      Width = 3
      Height = 16
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 81
    Width = 723
    Height = 291
    Align = alClient
    Color = clBtnHighlight
    ParentBackground = False
    TabOrder = 1
    object ListView1: TListView
      Left = 1
      Top = 1
      Width = 721
      Height = 289
      Align = alClient
      Columns = <
        item
          Caption = #1040#1085#1090#1080#1074#1080#1088#1091#1089
          Width = 170
        end
        item
          Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
          Width = 295
        end
        item
          Caption = #1042#1077#1088#1089#1080#1103
          Width = 120
        end
        item
          Caption = #1044#1072#1090#1072' '#1086#1073#1085#1086#1074#1083#1077#1085#1080#1103
          Width = 126
        end>
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      GridLines = True
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      PopupMenu = PopupMenu1
      TabOrder = 0
      ViewStyle = vsReport
      OnColumnClick = ListView1ColumnClick
      OnCustomDrawItem = ListView1CustomDrawItem
      OnCustomDrawSubItem = ListView1CustomDrawSubItem
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 372
    Width = 723
    Height = 41
    Align = alBottom
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 2
    DesignSize = (
      723
      41)
    object LbResult: TLabel
      Left = 12
      Top = 12
      Width = 52
      Height = 18
      AutoSize = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Button1: TButton
      Left = 452
      Top = 6
      Width = 155
      Height = 25
      Anchors = [akTop, akRight]
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1074' '#1073#1088#1072#1091#1079#1077#1088#1077
      TabOrder = 0
      OnClick = OpenBrauzer
    end
    object Button2: TButton
      Left = 640
      Top = 5
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = #1042#1099#1093#1086#1076
      TabOrder = 1
      OnClick = exitResult
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 640
    Top = 127
    object N1: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      object N5: TMenuItem
        Caption = '&'#1042#1089#1102' '#1089#1090#1088#1086#1082#1091
        OnClick = PMCopyAllStroka
      end
      object TMenuItem
      end
      object N2: TMenuItem
        Caption = #1044#1077#1090#1077#1082#1090
        OnClick = PMCopyDetect
      end
      object N3: TMenuItem
        Caption = #1042#1077#1088#1089#1080#1102' '#1040#1085#1090#1080#1074#1080#1088#1091#1089#1072
      end
      object N4: TMenuItem
        Caption = #1044#1072#1090#1091' '#1086#1073#1085#1086#1074#1083#1077#1085#1080#1103
      end
    end
    object N8: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1088#1077#1079#1091#1083#1100#1090#1072#1090' '#1074' '#1092#1072#1081#1083
      OnClick = PmSaveToFile
    end
    object N7: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1072#1087#1082#1091' '#1089' '#1092#1072#1081#1083#1086#1084
      OnClick = PmOpenFilePatch
    end
    object N6: TMenuItem
      Caption = #1057#1074#1086#1081#1089#1090#1074#1072' '#1092#1072#1081#1083#1072
      OnClick = PMSvoistvoFile
    end
  end
  object SaveDialog: TSaveDialog
    Filter = 'Text|*.txt'
    Left = 640
    Top = 223
  end
end
