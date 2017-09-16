package EnergyModule.Action;

import com.intellij.openapi.actionSystem.*;
import com.intellij.openapi.command.WriteCommandAction;
import com.intellij.openapi.project.Project;
import com.intellij.openapi.vfs.VirtualFile;
import com.intellij.psi.*;


public class EnableTimePoints extends AnAction {

    public EnableTimePoints() {
        super("Text _Boxes");
    }

    @Override
    public void actionPerformed(AnActionEvent event) {
        final Project project = event.getRequiredData(CommonDataKeys.PROJECT);
        Runnable runnable = () -> {
            EnergyProfilerGenerator energyProfilerGenerator = new EnergyProfilerGenerator();
            /*energyProfilerGenerator.generate(psiFile, project);*/
            VirtualFile sourceDir = EnergyProfilerGenerator.getModuleSource(project);
            if (sourceDir != null) {
                PsiDirectory directory = PsiManager.getInstance(project).findDirectory(sourceDir);
                PsiDirectory newDirectory = energyProfilerGenerator.createDirectory(directory, "TimeKeeper");
                PsiDirectory newPackage = energyProfilerGenerator.createPackage(newDirectory);
                if (newPackage.findFile("TimeKeeper.java") == null) {
                    PsiClass timeKeeper = JavaDirectoryService.getInstance().createClass(newPackage, "TimeKeeper");
                    energyProfilerGenerator.generate(timeKeeper, project);
                }
                EnergyProfilerGenerator.isEnabled = true;
            }
        };
        //Making the replacement
        WriteCommandAction.runWriteCommandAction(project, runnable);
    }


}
