package EnergyModule.LineMarker;

import com.intellij.codeInsight.daemon.RelatedItemLineMarkerProvider;
import com.intellij.codeInsight.daemon.RelatedItemLineMarkerInfo;
import com.intellij.codeInsight.navigation.NavigationGutterIconBuilder;
import com.intellij.psi.*;
import com.intellij.psi.impl.source.tree.java.PsiReferenceExpressionImpl;
import org.jetbrains.annotations.NotNull;

import java.util.*;

/**
 * Created by kd913 on 28/04/17.
 */
public class EnergyLineMarkerProvider extends RelatedItemLineMarkerProvider{

    @Override
    protected void collectNavigationMarkers(@NotNull PsiElement element,
                                            Collection<? super RelatedItemLineMarkerInfo> result) {
        if (element instanceof PsiReferenceExpressionImpl) {
            PsiReferenceExpressionImpl method = (PsiReferenceExpressionImpl) element;
            PsiElement identifier = method.getReferenceNameElement();
            if (identifier instanceof PsiIdentifier) {
                String value = identifier.getText();
                if (value.startsWith("insertTimePoint")) {
                    NavigationGutterIconBuilder<PsiElement> builder =
                            NavigationGutterIconBuilder.create(EnergyModule.FileIcon.EnergyIcon.FILE)
                                    .setTargets()
                                    .setTooltipText("Energy Point");
                    result.add(builder.createLineMarkerInfo(element));
                }

            }
        }
    }
}
