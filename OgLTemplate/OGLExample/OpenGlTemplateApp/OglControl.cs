using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace OpenGlTemplateApp
{
    public partial class OglControl : UserControl
    {
        int oglcontext=0;
        bool enablePaint;
        public bool EnablePaint
        {
            get { return enablePaint; }
            set { 
                enablePaint = value;  
                if(enablePaint)
                {
                    if (oglcontext == 0) oglcontext = DllImportFunctions.InitOpenGL((int)Handle);
                }
                }
        }
        public OglControl()
        {
            InitializeComponent();
        }
        protected override void OnPaintBackground(PaintEventArgs pevent) { }

        int renderParameter;
        public int RenderParameter
        {
            get { return renderParameter; }
            set { renderParameter = value; }
        }
        private void OglControl_Paint(object sender, PaintEventArgs e)
        {
            if(oglcontext != 0)  DllImportFunctions.Render((int)Handle, oglcontext, Width, Height, RenderParameter);
        }        

    }
}
public class DllImportFunctions
{
    [DllImport("OpenGlCppCode.Dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "InitOpenGL")]
    public static extern int InitOpenGL(int hdc);

    [DllImport("OpenGlCppCode.Dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Render")]
    public static extern void Render(int hdc, int oglcontext, int w, int h, int type);

}
