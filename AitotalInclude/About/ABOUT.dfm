object AboutBox: TAboutBox
  Left = 200
  Top = 108
  BorderStyle = bsDialog
  Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
  ClientHeight = 209
  ClientWidth = 343
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 343
    Height = 172
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object ProductName: TLabel
      Left = 8
      Top = 18
      Width = 102
      Height = 16
      Caption = #1048#1084#1103' '#1087#1088#1086#1075#1088#1072#1084#1084#1099
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      IsControl = True
    end
    object Version: TLabel
      Left = 8
      Top = 40
      Width = 47
      Height = 16
      Caption = #1042#1077#1088#1089#1080#1103
      IsControl = True
    end
    object Copyright: TLabel
      Left = 8
      Top = 82
      Width = 57
      Height = 16
      Caption = 'Copyright'
      IsControl = True
    end
    object Comments: TLabel
      Left = 8
      Top = 104
      Width = 89
      Height = 16
      Caption = #1050#1086#1084#1084#1077#1085#1090#1072#1088#1080#1081
      WordWrap = True
      IsControl = True
    end
    object Label1: TLabel
      Left = 128
      Top = 18
      Width = 37
      Height = 16
      Caption = 'Aitotal'
    end
    object Label2: TLabel
      Left = 128
      Top = 40
      Width = 17
      Height = 16
      Caption = '3.2'
    end
    object Label3: TLabel
      Left = 131
      Top = 82
      Width = 138
      Height = 16
      Caption = '(c) Aibolit aka Alex1983'
    end
    object Label4: TLabel
      Left = 127
      Top = 104
      Width = 204
      Height = 16
      Caption = #1044#1072#1085#1085#1072#1103' '#1087#1088#1086#1075#1088#1072#1084#1084#1072' '#1085#1077' '#1103#1074#1083#1103#1077#1090#1089#1103
    end
    object Label5: TLabel
      Left = 128
      Top = 118
      Width = 141
      Height = 16
      Caption = #1079#1072#1084#1077#1085#1086#1081' '#1072#1085#1090#1080#1074#1080#1088#1091#1089#1072'.'
    end
    object Label6: TLabel
      Left = 8
      Top = 60
      Width = 35
      Height = 16
      Caption = #1044#1072#1090#1072' '
    end
    object Label7: TLabel
      Left = 127
      Top = 60
      Width = 62
      Height = 16
      Caption = '13.12.2015'
    end
    object Label8: TLabel
      Left = 8
      Top = 144
      Width = 224
      Height = 16
      Caption = #1041#1083#1072#1075#1086#1076#1072#1088#1085#1086#1089#1090#1100' '#1040#1083#1077#1082#1089#1072#1085#1076#1077#1088', regist.'
    end
  end
  object OKButton: TButton
    Left = 127
    Top = 178
    Width = 81
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
