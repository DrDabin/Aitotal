object MyOptionsForm: TMyOptionsForm
  Left = 512
  Top = 192
  BorderIcons = [biSystemMenu]
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072
  ClientHeight = 412
  ClientWidth = 418
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 418
    Height = 379
    Align = alClient
    TabOrder = 0
    object MyOptionsPage: TPageControl
      Left = 1
      Top = 1
      Width = 416
      Height = 377
      ActivePage = MyOptionsNetworc
      Align = alClient
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object MyOptionsGlobal: TTabSheet
        Caption = #1054#1073#1097#1080#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
        object Panel2: TPanel
          Left = 0
          Top = 0
          Width = 408
          Height = 349
          Align = alClient
          Color = cl3DLight
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Microsoft Sans Serif'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
          object Label1: TLabel
            Left = 8
            Top = 8
            Width = 223
            Height = 16
            Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1092#1072#1081#1083#1086#1074' '#1087#1088#1080' '#1079#1072#1075#1088#1091#1079#1082#1077
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label2: TLabel
            Left = 8
            Top = 22
            Width = 84
            Height = 16
            Caption = #1053#1077' '#1073#1086#1083#1077#1077' 20. '
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label3: TLabel
            Left = 8
            Top = 132
            Width = 237
            Height = 17
            AutoSize = False
            Caption = #1044#1077#1081#1089#1090#1074#1080#1077' '#1087#1088#1080' '#1079#1072#1082#1088#1099#1090#1080#1080' '#1087#1088#1086#1075#1088#1072#1084#1084#1099':'
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label4: TLabel
            Left = 8
            Top = 44
            Width = 270
            Height = 16
            Caption = #1054#1073#1097#1077#1077' '#1082#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1087#1088#1086#1074#1077#1088#1103#1077#1084#1099#1093' '#1092#1072#1081#1083#1086#1074' '
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label5: TLabel
            Left = 8
            Top = 60
            Width = 84
            Height = 16
            Caption = #1053#1077' '#1073#1086#1083#1077#1077' 50. '
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label6: TLabel
            Left = 258
            Top = 280
            Width = 20
            Height = 16
            Caption = 'MB'
          end
          object OptLabLang: TLabel
            Left = 297
            Top = 135
            Width = 72
            Height = 16
            AutoSize = False
            Caption = #1071#1079#1099#1082
          end
          object OpTionFileCount: TEdit
            Left = 296
            Top = 5
            Width = 63
            Height = 24
            TabOrder = 0
            OnChange = OpTionFileCountChange
            OnKeyPress = OpTionFileCountKeyPress
          end
          object RBExit: TRadioButton
            Left = 8
            Top = 155
            Width = 145
            Height = 17
            Caption = #1047#1072#1082#1088#1099#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object RBTrei: TRadioButton
            Left = 8
            Top = 173
            Width = 137
            Height = 17
            Caption = #1057#1074#1077#1088#1085#1091#1090#1100' '#1074' '#1090#1088#1077#1081
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object OptionThreadCount: TEdit
            Left = 297
            Top = 41
            Width = 62
            Height = 24
            TabOrder = 1
            OnChange = OpTionCountThreadChange
          end
          object OptApikey: TLabeledEdit
            Left = 8
            Top = 105
            Width = 351
            Height = 24
            EditLabel.Width = 90
            EditLabel.Height = 16
            EditLabel.Caption = #1050#1083#1102#1095' API key'
            EditLabel.Font.Charset = RUSSIAN_CHARSET
            EditLabel.Font.Color = clWindowText
            EditLabel.Font.Height = -13
            EditLabel.Font.Name = 'MS Reference Sans Serif'
            EditLabel.Font.Style = []
            EditLabel.ParentFont = False
            EditLabel.ParentShowHint = False
            EditLabel.ShowHint = True
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Reference Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object ErrorArchiv: TCheckBox
            Left = 8
            Top = 231
            Width = 323
            Height = 17
            Hint = #1042#1099#1074#1086#1076#1080#1090#1100' '#1091#1074#1077#1076#1086#1084#1083#1077#1085#1080#1077' '#1077#1089#1083#1080' '#1087#1088#1086#1080#1079#1086#1096#1083#1072' '#1086#1096#1080#1073#1082#1072' '#1088#1072#1089#1087#1072#1082#1086#1074#1082#1080' '#1072#1088#1093#1080#1074#1072'?'
            HelpContext = 7
            Caption = #1059#1074#1077#1076#1086#1084#1083#1077#1085#1080#1077' '#1087#1088#1080' '#1086#1096#1080#1073#1082#1077' '#1088#1072#1089#1087#1072#1082#1086#1074#1082#1080' '#1072#1088#1093#1080#1074#1072
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 6
            WordWrap = True
          end
          object ToolsReset: TButton
            Left = 125
            Top = 323
            Width = 75
            Height = 25
            Caption = #1057#1073#1088#1086#1089#1080#1090#1100
            TabOrder = 8
            OnClick = OptRest
          end
          object RunAplicatDubl: TCheckBox
            Left = 8
            Top = 208
            Width = 265
            Height = 17
            Caption = #1056#1072#1079#1088#1077#1096#1080#1090#1100' '#1079#1072#1087#1091#1089#1082' '#1085#1077#1089#1082#1086#1083#1100#1082#1080#1093' '#1082#1086#1087#1080#1081
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object CBBallonhint: TCheckBox
            Left = 8
            Top = 254
            Width = 361
            Height = 17
            Caption = #1042#1099#1074#1086#1076#1080#1090#1100' '#1089#1086#1086#1073#1097#1077#1085#1080#1077' '#1086#1073' '#1086#1082#1086#1085#1095#1072#1085#1080#1080' '#1087#1088#1086#1074#1077#1088#1082#1080' '#1092#1072#1081#1083#1072'.'
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object LEMaxFileSize: TLabeledEdit
            Left = 215
            Top = 272
            Width = 37
            Height = 24
            EditLabel.Width = 196
            EditLabel.Height = 16
            EditLabel.Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1081' '#1088#1072#1079#1084#1077#1088' '#1092#1072#1081#1083#1072
            LabelPosition = lpLeft
            TabOrder = 9
          end
          object OptComboLang: TComboBox
            Left = 258
            Top = 157
            Width = 145
            Height = 24
            Style = csDropDownList
            ItemIndex = 0
            TabOrder = 10
            Text = 'Russian'
            OnClick = OptComboLangClick
            Items.Strings = (
              'Russian'
              'English')
          end
        end
      end
      object MyOptionsNetworc: TTabSheet
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1089#1077#1090#1080
        ImageIndex = 1
        object Panel3: TPanel
          Left = 0
          Top = 0
          Width = 408
          Height = 349
          Align = alClient
          Color = cl3DLight
          ParentBackground = False
          TabOrder = 0
          object TypSocket: TLabel
            Left = 32
            Top = 198
            Width = 73
            Height = 16
            Caption = #1058#1080#1087' '#1089#1086#1082#1077#1090#1072
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Microsoft Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object OptProxi: TLabeledEdit
            Left = 8
            Top = 64
            Width = 121
            Height = 21
            EditLabel.Width = 70
            EditLabel.Height = 13
            EditLabel.Caption = #1055#1088#1086#1082#1089#1080' '#1040#1076#1088#1077#1089
            TabOrder = 0
          end
          object OptIpPort: TLabeledEdit
            Left = 176
            Top = 64
            Width = 121
            Height = 21
            EditLabel.Width = 44
            EditLabel.Height = 13
            EditLabel.Caption = 'Ip - '#1087#1086#1088#1090
            TabOrder = 1
          end
          object OptProxiPassword: TLabeledEdit
            Left = 176
            Top = 123
            Width = 121
            Height = 21
            EditLabel.Width = 37
            EditLabel.Height = 13
            EditLabel.Caption = #1055#1072#1088#1086#1083#1100
            PasswordChar = '*'
            TabOrder = 3
          end
          object OptProxiLogin: TLabeledEdit
            Left = 8
            Top = 123
            Width = 121
            Height = 21
            EditLabel.Width = 30
            EditLabel.Height = 13
            EditLabel.Caption = #1051#1086#1075#1080#1085
            TabOrder = 2
          end
          object BtTestProxi: TButton
            Left = 120
            Top = 276
            Width = 97
            Height = 25
            Caption = #1055#1088#1086#1090#1077#1089#1090#1080#1088#1086#1074#1072#1090#1100
            TabOrder = 4
            OnClick = TestProxi
          end
          object ComboBox1: TComboBox
            Left = 120
            Top = 197
            Width = 145
            Height = 21
            Style = csDropDownList
            ItemIndex = 0
            TabOrder = 5
            Text = 'http. https'
            Items.Strings = (
              'http. https'
              'socks4'
              'socks5')
          end
          object RadioGroup1: TRadioGroup
            Left = 0
            Top = 0
            Width = 321
            Height = 41
            Color = cl3DLight
            ParentBackground = False
            ParentColor = False
            TabOrder = 6
          end
          object RBOptionsIE: TRadioButton
            Left = 0
            Top = 8
            Width = 122
            Height = 17
            Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1072#1082' '#1091' IE'
            TabOrder = 7
            OnClick = RBOptionsIEClick
          end
          object RBProxy: TRadioButton
            Left = 143
            Top = 8
            Width = 122
            Height = 17
            Caption = #1048#1089#1087#1086#1100#1079#1086#1074#1072#1090#1100' '#1087#1088#1086#1082#1089#1080
            TabOrder = 8
            OnClick = RBProxyClick
          end
        end
      end
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 379
    Width = 418
    Height = 33
    Align = alBottom
    TabOrder = 1
    object Button1: TButton
      Left = 130
      Top = 6
      Width = 75
      Height = 25
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      TabOrder = 0
      OnClick = OptionSave
    end
  end
  object ActionList1: TActionList
    Left = 127
    Top = 84
    object MuOptionsSeve: TAction
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    end
    object MyOptionsReadIni: TAction
      Caption = #1063#1080#1090#1072#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
    end
  end
end
